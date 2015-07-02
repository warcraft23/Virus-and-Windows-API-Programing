#include <windows.h>

int main(int argc, char *argv[])
{
    DWORD dwBase;
    
    PIMAGE_DOS_HEADER pDosHeader;
    PIMAGE_NT_HEADERS pNtHeaders;
    PIMAGE_FILE_HEADER pFileHeader;
    PIMAGE_OPTIONAL_HEADER32 pOptHeader;

    DWORD dwRVAIAT;
    DWORD dwSizeOfIAT;

    PDWORD dwpIAT, dwpImpAddr;
    PIMAGE_IMPORT_DESCRIPTOR pImpTbl, pImpDesc;

    PIMAGE_THUNK_DATA pthunk, pthunk2;
    PIMAGE_IMPORT_BY_NAME pOrdinalName;
    int i;

    dwBase = (DWORD)GetModuleHandleA(NULL);

    pDosHeader = (PIMAGE_DOS_HEADER)dwBase;

    pNtHeaders = (PIMAGE_NT_HEADERS)(dwBase + pDosHeader->e_lfanew);

    pOptHeader = &(pNtHeaders->OptionalHeader);

    dwRVAIAT = pOptHeader->DataDirectory[12].VirtualAddress;

    dwSizeOfIAT = pOptHeader->DataDirectory[12].Size;

  	dwpIAT = (PDWORD)(dwBase + dwRVAIAT);

    printf("=============================================================\n");

    for (dwpImpAddr = dwpIAT; 
            (DWORD)dwpImpAddr < ((DWORD)dwpIAT + dwSizeOfIAT); 
            dwpImpAddr++) {
        printf("0x%08X ==> 0x%08X\n", dwpImpAddr, *dwpImpAddr);
    }

    if (0) MessageBoxA(0,"","",0);
    return 0;
}
