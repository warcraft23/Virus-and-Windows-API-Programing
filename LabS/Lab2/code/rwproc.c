#include <windows.h>

int main(int argc, char *argv[])
{
	int pid = 0;
	HANDLE hproc = 0;
	char buf[100] = {0x0};
	PBYTE pRemote = (PBYTE)0x00408000;
	int nRead, nWrite;
	char *s = "I'm hacked";
	DWORD old;

	if (argc < 2) {
		printf("Usage: %s pid\n", argv[0]);
		return 1;
	}
	pid = atoi(argv[1]);
	if (pid <= 0) {
		printf("[E]: pid is negative !\n"); 
		return 2;
	}

	hproc = OpenProcess(PROCESS_QUERY_INFORMATION 
		| PROCESS_VM_READ
		| PROCESS_VM_WRITE
		| PROCESS_VM_OPERATION, 0, pid);

	if (hproc == 0) {
		printf("[E]: Process (%d) cannot be opened !\n", pid);
		return 2;
	}
	printf("[I]: Process (%d) is opened --> 0x%08x\n", pid, hproc);

	if (!ReadProcessMemory(hproc, 
		pRemote, (PVOID)buf, 20, &nRead)) {
		printf("[E]: Read data from process failed at 0x%08x!\n", pRemote);
		return 2;
	}
	printf("[I]: Read data from process (%d) from 0x%08x --> %s \n", pid, pRemote, buf);

	if (!WriteProcessMemory(hproc, 
		pRemote, s, strlen(s)+1, &nWrite)) {
		printf("[E]: Write data failed at 0x%08x!\n", pRemote);
	}
	printf("[I]: Write data (size: %d) at 0x%08x.\n", nWrite, pRemote);

	if (!CloseHandle(hproc)) {
		printf("[E]: Process (%d) cannot be closed !\n", pid);
		return 2;
	};
	printf("[I]: Process (%d) is closed. \n", pid);
}


//	if (VirtualProtectEx(hproc, 
//		pRemote, strlen(s), PAGE_READWRITE, &old)==0) {
//		printf("[E]: Modify page flag at 0x%08x\n", pRemote);
//		return 2;
//	};
//	printf("[I]: Page flag at 0x%08x is modified(%d).\n", pRemote);
