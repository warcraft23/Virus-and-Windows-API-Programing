#include <windows.h>
#include <stdio.h>
#include <winuser.h>

PBYTE pCode, pOrignCode;
DWORD dwSizeOfCode;

DWORD  __stdcall _newproc(HWND hwnd,      // handle to window         
                        UINT uMsg,          // message identifier       
                        WPARAM wParam,     // first message parameter 
                        LPARAM lParam );
void _addr_HandleNotepad();
void _addr_SetWindowLongA();
void _addr_oldnewproc();
void _addr_CallWindowProcA();

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

        lea  ecx, [ebx + _newproc]
        push ecx
        push -4
        mov  ecx, [ebx + _addr_HandleNotepad]
        push ecx
        lea  ecx, [ebx + _addr_SetWindowLongA]
        call dword ptr [ecx]

        //save old proc of hwnd
        lea  ecx, [ebx + _addr_oldnewproc]
        mov dword ptr [ecx], eax

        pop  ebx
        mov  esp, ebp
        pop  ebp
        ret
    }
}


__declspec(naked) DWORD __stdcall _newproc(HWND hwnd,      // handle to window         
                        UINT uMsg,          // message identifier       
                        WPARAM wParam,     // first message parameter 
                        LPARAM lParam )    // second message parameter
{
    __asm{
        push    ebp
        mov     ebp,esp
        push    ebx
        push    esi
        push    edi
        call    _delta2
_delta2:
        pop     ebx
        sub     ebx,    offset _delta2
        mov     ecx,    dword ptr [ebp + 0x0C]  // ecx <- uMsg
        mov     edx,    dword ptr [ebp + 0x10]  // edx <- wParam
        mov     edi,    dword ptr [ebp + 0x14]  // edi <- lParam
_cont0:
        cmp     ecx,    WM_CHAR
        jne     _contn
        cmp     edx,    'A'
        jne     _contn
        mov     edx,    'B'
_contn:
        push    edi
        push    edx
        push    ecx
        mov  ecx, [ebx + _addr_HandleNotepad]
        push ecx
        //method 1
        //mov     ecx, dword ptr [ebp + 0x08]
        //push    ecx
        //method 2
        mov     ecx, [ebx + _addr_oldnewproc]
        push    ecx
        // 在这里填入代码，调用旧的消息处理函数
        // method 1
        // lea     ecx,  [ebx + _addr_CallWindowProcA]
        // call    dword ptr [ecx]
        // method 2
        mov     ecx,  [ebx + _addr_CallWindowProcA]
        call    ecx
_ret2:
        pop     edi
        pop     esi
        pop     ebx
        mov     esp, ebp
        pop     ebp
        ret     0x10

    }
}

__declspec(naked) void _addr_SetWindowLongA()
{
    __asm {
        _emit 0xAA
        _emit 0xBB
        _emit 0xAA
        _emit 0xEE
    }
}


__declspec(naked) void _addr_HandleNotepad()
{
    __asm {
        _emit 0xAA
        _emit 0xBB
        _emit 0xAA
        _emit 0xEE
    }
}

__declspec(naked) void _addr_oldnewproc()
{
    __asm {
        _emit 0xAA
        _emit 0xBB
        _emit 0xAA
        _emit 0xEE
    }
}

/*
LRESULT WINAPI CallWindowProc(
  _In_  WNDPROC lpPrevWndFunc,
  _In_  HWND hWnd,
  _In_  UINT Msg,
  _In_  WPARAM wParam,
  _In_  LPARAM lParam
);
*/

__declspec(naked) void _addr_CallWindowProcA()
{
    __asm {
        _emit 0xAA
        _emit 0xBB
        _emit 0xAA
        _emit 0xEE
    }
}

__declspec(naked) void code_end()
{
    __asm _emit 0xCC
}


DWORD make_code(DWORD pid, HWND hwnd)
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
    
    //Get SetWindowLongA
    hModule = LoadLibrary("user32.dll");
    if (hModule == NULL) {
        printf("[E]: user32.dll cannot be loaded. \n");
        return 0;
    }
    func_addr = (DWORD)GetProcAddress(hModule, "SetWindowLongA");

    if (func_addr == 0) {
        printf("[E]: SetWindowLongA not found. \n");
        return 0;
    }
    else
        printf("\nThe address of SetWindowLongA: 0x%08x\n",func_addr );
    off = (DWORD)pCode - (DWORD)pOrignCode;
    *(PDWORD)((PBYTE)_addr_SetWindowLongA + off) = func_addr;

    //Change handle of notepad
    printf("The handle: 0x%08x\n",hwnd );
    *(PDWORD)((PBYTE)_addr_HandleNotepad + off) = hwnd;


    //change the address of CallWindowProcA
    func_addr = (DWORD)GetProcAddress(hModule, "CallWindowProcA");
    printf("The address of CallWindowProcA:0x%08x\n",func_addr);
    *(PDWORD)((PBYTE)_addr_CallWindowProcA + off) = func_addr;

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

    return base_addr;
}



int main(int argc, char *argv[])
{
    
    HWND        hMainWnd, hWnd;
    DWORD       hproc, pid;

    //Get pid of notepad.exe
    hMainWnd = FindWindow ("notepad", NULL);
    hWnd = GetWindow(hMainWnd, GW_CHILD);
    GetWindowThreadProcessId(hWnd, &pid);

    make_code(pid,hWnd);
    printf("The context of ret: 0x%08x",inject_code(pid) );

    return 0;
}