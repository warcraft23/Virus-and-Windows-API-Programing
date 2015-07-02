#include <windows.h>

char code[] = {
0x68, 0xCC, 0xDD, 0xAA, 0xBB,
0xB8, 0x55, 0xDC, 0xC1, 0x76,
0xFF, 0xD0,
0xC3};
char *hstr = "hello.exe";
int main()
{
   int PID = 4180, TID=0;
   DWORD hproc, hthrd;
   int rstr, rcode, base_addr, old, numx;
   
   printf("code addr: 0x%08x\n", code);
   hproc = OpenProcess(
          PROCESS_CREATE_THREAD  | PROCESS_QUERY_INFORMATION
        | PROCESS_VM_OPERATION   | PROCESS_VM_WRITE 
        | PROCESS_VM_READ, FALSE, PID);
   rstr = (PBYTE) VirtualAllocEx(hproc, 
        0, 12, MEM_COMMIT, PAGE_READWRITE);
   WriteProcessMemory(hproc, rstr, hstr, 10, &numx);
   __asm {
     mov ebx, offset code
     mov eax, dword ptr [rstr]
     mov [ebx+0x1], eax }
   rcode = (PBYTE) VirtualAllocEx(hproc, 
        0, 20, MEM_COMMIT, PAGE_EXECUTE_READWRITE);	
   WriteProcessMemory(hproc, rcode, code, sizeof(code), &numx);
   hthrd = CreateRemoteThread(hproc, 
      NULL, 0, (LPTHREAD_START_ROUTINE)rcode,
            0, 0 , &TID);
   WaitForSingleObject(hthrd, 0);  
   GetExitCodeThread(hthrd, &base_addr);
   printf("base addr of %s: 0x%08x", hstr, base_addr);
}