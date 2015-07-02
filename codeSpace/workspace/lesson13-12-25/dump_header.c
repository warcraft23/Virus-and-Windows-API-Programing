#include <windows.h>
int main(int argc, char *argv[])
{ 
    char 								    *signature;
    DWORD 							    dwBase;
    PIMAGE_DOS_HEADER 			pDosHeader;
    PIMAGE_NT_HEADERS 			pNtHeaders;
    PIMAGE_FILE_HEADER 			pFileHeader;
    PIMAGE_OPTIONAL_HEADER32 	pOptHeader;

    dwBase = (DWORD)GetModuleHandleA(NULL);
    printf("Base Address: 0x%08x\n", dwBase);
    pDosHeader = (PIMAGE_DOS_HEADER)dwBase;
    printf("e_lfanew: 0x%08x\n", pDosHeader->e_lfanew);
    pNtHeaders = (PIMAGE_NT_HEADERS)(dwBase + pDosHeader->e_lfanew);
    printf("PE NT Headers Address: 0x%08x\n", pNtHeaders);
    signature = (char *)&(pNtHeaders->Signature);
    printf("Signature: %c%c%c%c\n", *signature, *(signature+1), 
            *(signature+2),  *(signature+3));
    pOptHeader = &(pNtHeaders->OptionalHeader);
    printf("BaseOfCode: 0x%08x\n", pOptHeader->BaseOfCode);
    printf("ImageBase: 0x%08x\n", pOptHeader->ImageBase);
    printf("Subsystem: 0x%04x\n", pOptHeader->Subsystem);
    return 0;
}
