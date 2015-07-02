#include <windows.h>

/*******************************************************************************
 * This section defines global constants and global variables that will be
 * used in the analysis of PE files.
 ******************************************************************************/
#define MAX_SECTION_NUM     96
#define ONE_PAGE_SIZE   0x1000

DWORD dwBaseAddress;

PIMAGE_DOS_HEADER pDosHeader;
PCHAR             pDosStub;
DWORD             dwDosStubSize;
DWORD             dwDosStubOffset;

char *signature;
PIMAGE_NT_HEADERS        pNtHeaders;
PIMAGE_FILE_HEADER       pFileHeader;
PIMAGE_OPTIONAL_HEADER32 pOptHeader;

PIMAGE_SECTION_HEADER   pSecHeaders;
PIMAGE_SECTION_HEADER	pSecHeader[MAX_SECTION_NUM];
WORD                    wSecNum;
PCHAR                   pSecData[MAX_SECTION_NUM];

void DumpSectionTable()
{
    int i;
    PIMAGE_SECTION_HEADER pSecHdrStart, pSecHdr;
    WORD wSecNum = 0;
    PIMAGE_SECTION_HEADER pSecHeader[64];
    pSecHdrStart = (PIMAGE_SECTION_HEADER)
       (dwBaseAddress + (pDosHeader->e_lfanew) + sizeof(IMAGE_NT_HEADERS));
    wSecNum = pFileHeader->NumberOfSections;
    printf("Number Of Sections: %d\n", wSecNum);

    for(i = 0, pSecHdr = pSecHdrStart ; i < wSecNum; i++, pSecHdr++) {
        pSecHeader[i] = (PIMAGE_SECTION_HEADER)pSecHdr;
        printf("Section #%d\n",i);
        printf("    Name: %c%c%c%c%c%c%c%c\n", pSecHdr->Name[0],pSecHdr->Name[1],
            pSecHdr->Name[2],pSecHdr->Name[3],pSecHdr->Name[4],pSecHdr->Name[5],
            pSecHdr->Name[6],pSecHdr->Name[7]);
        printf("    Virtual Size: 0x%08X\n", 
            pSecHdr->Misc.VirtualSize);
        printf("    Virtual Address: 0x%08X\n", 
            pSecHdr->VirtualAddress);
    }

    return;
}

int main(int argc, char *argv[])
{
    
    dwBaseAddress = (DWORD)GetModuleHandleA(NULL);

    printf("Base Address: 0x%08x\n", dwBaseAddress);
    
    pDosHeader = (PIMAGE_DOS_HEADER)dwBaseAddress;
    
    printf("e_lfanew: 0x%08x\n", pDosHeader->e_lfanew);
    
    pNtHeaders = (PIMAGE_NT_HEADERS)(dwBaseAddress + pDosHeader->e_lfanew);
    
    printf("PE NT Headers Address: 0x%08x\n", pNtHeaders);

    signature = (char *)&(pNtHeaders->Signature);
    
    printf("Signature: %c%c%c%c\n", *signature, *(signature+1),*(signature+2), *(signature+3));

    pFileHeader = &(pNtHeaders->FileHeader);
    
    pOptHeader = &(pNtHeaders->OptionalHeader);
    
    printf("BaseOfCode: 0x%08x\n", pOptHeader->BaseOfCode);
  
    printf("ImageBase: 0x%08x\n", pOptHeader->ImageBase);

    printf("Subsystem: 0x%04x\n", pOptHeader->Subsystem);

    DumpSectionTable();
    
    return 0;
}
