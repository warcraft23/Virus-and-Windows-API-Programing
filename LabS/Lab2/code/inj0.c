#include <windows.h>

BYTE code[]={0x33,0xc0,0xc3};

int main(int argc, char *argv[])
{	
    int     PID         = 0;
    HANDLE  hProcess    = 0; 
    PBYTE   pCodeRemote = NULL;
    DWORD   dwNumBytesXferred = 0;
    
    PBYTE   pCode      = NULL;
    DWORD   dwSizeOfCode = 0;
    
    HANDLE  hThread	   = 0;
    DWORD   dwThreadId = 0;
    int	    exitcode   = 0;

    if (argc < 2) {
        printf("Usage: %s pid\n", argv[0]);
        return -1;
    }
    PID = atoi(argv[1]);
    if (PID <= 0) {
        printf("[E]: pid should be greater than zero!\n"); 
        return -1;
    }
	
    pCode = (PBYTE)code;
    dwSizeOfCode = sizeof(code);

    printf("[I]: Opening remote process %d......", PID); 
    hProcess = OpenProcess(PROCESS_CREATE_THREAD 
        | PROCESS_QUERY_INFORMATION
        | PROCESS_VM_OPERATION 
        | PROCESS_VM_WRITE 
        | PROCESS_VM_READ,
        FALSE, PID);
        
    if (hProcess == NULL) {
        printf("failed.\n"); 
        return -1;
    }   
    printf("ok.\n");

    printf("[I]: Allocating remote memory with size of 0x%08x ......", 
        dwSizeOfCode);

    pCodeRemote = (PBYTE) VirtualAllocEx(hProcess, 
            0, 
            dwSizeOfCode, 
            MEM_COMMIT, 
            PAGE_EXECUTE_READWRITE);		
    if (pCodeRemote == NULL) {
        printf("failed.\n");
        CloseHandle(hProcess);
        return -1;
    }
    printf("ok at 0x%08x.\n", pCodeRemote);

    printf("[I]: Writing code ......");
    if (WriteProcessMemory(hProcess, 
            pCodeRemote, 
            pCode, 
            dwSizeOfCode, 
            &dwNumBytesXferred) == 0) {
        printf("failed.\n");
        VirtualFreeEx(hProcess, pCodeRemote,
                dwSizeOfCode, MEM_RELEASE);
        CloseHandle(hProcess);
        return -1;
    };
    printf("ok (%d bytes were written).\n", dwNumBytesXferred);
        
    printf("[I]: Creating a remote thread ......");
    hThread = CreateRemoteThread(hProcess, NULL, 0, 
            (LPTHREAD_START_ROUTINE) pCodeRemote,
            pCodeRemote, 0 , &dwThreadId);
    if (hThread == 0) {
        printf("failed.\n");
        if ( pCodeRemote != 0 )	
            VirtualFreeEx(hProcess, pCodeRemote, 0, MEM_RELEASE);
        if ( hThread != 0 )			
            CloseHandle(hThread);
        return -1;
    }
    printf("ok.\n");
 
    printf("[I]: Waiting the remote thread ......");
    WaitForSingleObject(hThread, INFINITE);
    GetExitCodeThread(hThread, (PDWORD) &exitcode);
    printf("exited with 0x%08X\n", exitcode);
 
    VirtualFreeEx(hProcess, pCodeRemote, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return 0;
}