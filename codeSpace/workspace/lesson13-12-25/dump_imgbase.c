#include <windows.h>

int main(int argc, char *argv[])
{
    DWORD dwBaseAddress;
    PIMAGE_DOS_HEADER pDosHeader;
    PIMAGE_NT_HEADERS pNtHeaders;
    PIMAGE_OPTIONAL_HEADER32 pOptHeader;
    
    
    dwBaseAddress = (DWORD) GetModuleHandleA(NULL);
    if (dwBaseAddress == NULL) {
        printf("[E]: %s cannot be found.", argv[0]);
        return -1;
    }
    pDosHeader = (PIMAGE_DOS_HEADER)dwBaseAddress;

    pNtHeaders = (PIMAGE_NT_HEADERS)(
                         dwBaseAddress + pDosHeader->e_lfanew);

    pOptHeader = &(pNtHeaders->OptionalHeader);

    printf("ImageBase : 0x%08x\n", pOptHeader->ImageBase);
    printf("SizeOfCode : 0x%08x\n", pOptHeader->SizeOfCode);
    
    return 0;
 }
 