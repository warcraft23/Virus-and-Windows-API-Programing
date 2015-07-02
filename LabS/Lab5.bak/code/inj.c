#include <windows.h>
#include <stdio.h>
#include <Tlhelp32.h>
PBYTE pCode, pOrignCode;
DWORD dwSizeOfCode;

void _GetBaseKernel32();
void _GetGetProcAddrBase();

__declspec(naked) void code_start()
{
    __asm {
        push ebp
        mov  ebp, esp
        push ebx


        call _delta
_delta:
        pop  ebx
        sub  ebx, offset _delta

        //lea  ecx, [ebx + _GetBaseKernel32]
        //call dword ptr ss:[ecx]
        call offset _GetBaseKernel32
        
        push eax
        //lea  ecx, [ebx + _GetGetProcAddrBase]
        //call dword ptr [ecx]
        call offset _GetGetProcAddrBase

        pop  ebx
        mov  esp, ebp
        pop  ebp
        ret
  }
}

//被注入代码可以调用下面的代码得到kernel32.dll在远程进程中的基地址
__declspec(naked) void _GetBaseKernel32()
{
    __asm {
        push    ebp
        mov     ebp, esp
        push    esi
        push    edi
        xor     ecx, ecx                    // ECX = 0
        mov     esi, fs:[0x30]              // ESI = &(PEB) ([FS:0x30])
        mov     esi, [esi + 0x0c]           // ESI = PEB->Ldr
        mov     esi, [esi + 0x1c]           // ESI = PEB->Ldr.InInitOrder
_next_module:
        mov     eax, [esi + 0x08]           // EBP = InInitOrder[X].base_address
        mov     edi, [esi + 0x20]           // EBP = InInitOrder[X].module_name (unicode)
        mov     esi, [esi]                  // ESI = InInitOrder[X].flink (next module)
        cmp     [edi + 12*2], cx            // modulename[12] == 0 ?
        jne     _next_module                 // No: try next module.
        pop     edi
        pop     esi
        mov     esp, ebp
        pop     ebp
        ret
    }
}

//被注入代码可以调用下面的代码分析kernel32.dll的导出表，得到GetProcAddress在远程进程中的函数。
__declspec(naked) void _GetGetProcAddrBase()
{
    __asm {
        push    ebp
        mov     ebp, esp
        push    edx
        push    ebx
        push    edi
        push    esi
        mov     ebx, [ebp+8]
        mov     eax, [ebx + 0x3c] // edi = BaseAddr, eax = pNtHeader
        mov     edx, [ebx + eax + 0x78]
        add     edx, ebx          // edx = Export Table (RVA)
        mov     ecx, [edx + 0x18] // ecx = NumberOfNames
        mov     edi, [edx + 0x20] //
        add     edi, ebx          // ebx = AddressOfNames
_search:
        dec     ecx
        mov     esi, [edi + ecx*4]
        add     esi, ebx
        mov     eax, 0x50746547 // "PteG"
        cmp     [esi], eax
        jne     _search
        mov     eax, 0x41636f72 //"Acor"
        cmp     [esi+4], eax
        jne     _search
        mov     edi, [edx + 0x24] //
        add     edi, ebx      // edi = AddressOfNameOrdinals
        mov     cx, word ptr [edi + ecx*2]  // ecx = GetProcAddress-orinal
        mov     edi, [edx + 0x1c] //
        add     edi, ebx      // edi = AddressOfFunction
        mov     eax, [edi + ecx*4]
        add     eax, ebx      // eax = GetProcAddress
        
        pop     esi
        pop     edi
        pop     ebx
        pop     edx
        
        mov     esp, ebp
        pop     ebp
        ret
    }
}

__declspec(naked) void code_end()
{
    __asm _emit 0xCC
}


DWORD make_code()
{
    int off; 
    DWORD func_addr;
    HMODULE hModule;
    __asm {
        mov edx, offset code_start
        mov dword ptr [pOrignCode], edx
        mov eax, offset code_end
        sub eax, edx
        mov dword ptr [dwSizeOfCode], eax
    }
    pCode = VirtualAlloc(NULL, dwSizeOfCode, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (pCode== NULL) {
        printf("[E]: VirtualAlloc failed\n");
        return 0;
    }
    printf("[I]: VirtualAlloc ok --> at 0x%08x.\n", pCode);
    for (off = 0; off<dwSizeOfCode; off++) {
        *(pCode+off) = *(pOrignCode+off);
    }
    printf("[I]: Copy code ok --> from 0x%08x to 0x%08x with size of 0x%08x.\n", 
        pOrignCode, pCode, dwSizeOfCode);
    
    return dwSizeOfCode;
}

int inject_code(DWORD pid)
{
    //MyCode ICY
    int PID = pid, TID=0;
    DWORD hproc, hthrd;
    int rcode, base_addr, numx;

    hproc = OpenProcess(
          PROCESS_CREATE_THREAD  | PROCESS_QUERY_INFORMATION
        | PROCESS_VM_OPERATION   | PROCESS_VM_WRITE 
        | PROCESS_VM_READ, FALSE, PID);

    rcode = (PBYTE) VirtualAllocEx(hproc, 
        0, dwSizeOfCode, MEM_COMMIT, PAGE_EXECUTE_READWRITE); 

    WriteProcessMemory(hproc, rcode, pCode, dwSizeOfCode, &numx);
    hthrd = CreateRemoteThread(hproc, 
        NULL, 0, (LPTHREAD_START_ROUTINE)rcode,
        0, 0 , &TID);
    WaitForSingleObject(hthrd, 0xffffffff);  
    GetExitCodeThread(hthrd, &base_addr);

    //printf("The addr of GetProcAddress: 0x%08x", base_addr);
    return base_addr;
}

DWORD GetProcessID(char *proc)
{
  BOOL    working=0;
  PROCESSENTRY32 lppe= {0};
  DWORD    targetPid=0;
  HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS ,0);

  if (hSnapshot)
  {
     lppe.dwSize=sizeof(lppe);
     working=Process32First(hSnapshot,&lppe);
     while (working)
     {
      if (_stricmp(lppe.szExeFile,proc)==0)//不区分大小写
      {
       targetPid=lppe.th32ProcessID;
       break;
      }
      working=Process32Next(hSnapshot,&lppe);
     }
  }
  CloseHandle( hSnapshot );
  return targetPid;
}

int main(int argc, char *argv[])
{
    DWORD pid = 0;

    pid=GetProcessID("hello.exe");
    make_code();

    printf("The addr of GetProcAddress: 0x%08x", inject_code(pid));
    
    printf("\nThe addr of GetProcAddress main: 0x%08x", GetProcAddress );
    
    return 0;
}