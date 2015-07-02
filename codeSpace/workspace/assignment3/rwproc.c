#include <windows.h>

int main(int argc, char *argv[])
{
    int pid = 0;
    HANDLE hproc = 0;
    int buf;
    int nRead, nWrite;
    char *s = "I'm hacked";

    //PBYTE pRemote = (PBYTE)0x00401000;//改0x00401000 位置的内容 
    PBYTE pRemote = (PBYTE)0x00409af9;//改内容、 
    //PBYTE pRemote = (PBYTE)0x6b0c8000;//改标题栏 

    if (argc < 2) {
        printf("Usage: %s pid\n", argv[0]);
        return -1;
    }
    pid = atoi(argv[1]);
    if (pid <= 0) {
        printf("[E]: pid must be positive (pid>0)!\n"); 
        return -2;
    }

    hproc = OpenProcess(PROCESS_QUERY_INFORMATION 
        | PROCESS_VM_READ
        | PROCESS_VM_WRITE
        | PROCESS_VM_OPERATION, 0, pid);

    if (hproc == 0) {
        printf("[E]: Process (%d) cannot be opened !\n", pid);
        return -2;
    }
    printf("[I]: Process (%d) is opened --> 0x%08x\n", pid, hproc);

    if (!ReadProcessMemory(hproc, 
        pRemote, &buf, 4, &nRead)) {
        printf("[E]: Read DWORD from remote process failed at 0x%08x!\n", pRemote);
    }
    else {
        printf("[I]: Read DWORD from remote process (%d) from 0x%08x --> 0x%08x \n", pid, pRemote, buf);
    }
    printf("nRead:%08x\n",nRead);

    if (!WriteProcessMemory(hproc, 
        pRemote, s, strlen(s)+1, &nWrite)) {
        printf("[E]: Write string to remote process failed at 0x%08x!\n", pRemote);
    } else {
        printf("[I]: Write string (size: %d) to remote process at 0x%08x.\n", nWrite, pRemote);
    }

    if (!CloseHandle(hproc)) {
        printf("[E]: Process (%d) cannot be closed !\n", pid);
        return 2;
    };
    printf("[I]: Process (%d) is closed. \n", pid);
    return 0;
}
//开启ollydbg
//运行程序，在while处设断点，跑到while处，
//出提示框，看压入参数地址，然后找对应 内存区域的值，
//找到对应区域后，从偏移地址，找到字符串buf中helloworld所在的地址，获得真实地址，然后修改此地址即可 
