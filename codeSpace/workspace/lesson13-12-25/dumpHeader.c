#include <windows.h>
int main(int argc,char *argv[]){
char *signature;
DWORD dwBase;
PIMAGE_DOS_HEADER pDosHeader;
PIMAGE_NT_HEADERS pNtHeaders;
PIMAGE_FILE_HEADER pFileHeader;
PIMAGE_OPTIONAL_HEADER32 pOptHeader;

dwBase=(DWORD)GetModuleHandleA(NULL);
printf("BaseAddr:0x%08x\n",dwBase);
pDosHeader=(PIMAGE_DOS_HEADER)dwBase;
pNtHeaders=(PIMAGE_NT_HEADERS)(dwBase+pDosHeader->e_lfanew);
pOptHeader=&(pNtHeaders->OptionalHeader);
printf("ImageBase : 0x%08x\n",pOptHeader->ImageBase);
return 0;
}