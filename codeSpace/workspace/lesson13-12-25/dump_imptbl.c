#include <windows.h>

int main(int argc, char *argv[])
{
    DWORD dwBase;
    
    PIMAGE_DOS_HEADER pDosHeader;
    PIMAGE_NT_HEADERS pNtHeaders;
    PIMAGE_FILE_HEADER pFileHeader;
    PIMAGE_OPTIONAL_HEADER32 pOptHeader;

    DWORD dwRVAImpTbl;
    DWORD dwSizeOfImpTbl;

    PIMAGE_IMPORT_DESCRIPTOR pImpTbl, pImpDesc;
    PIMAGE_THUNK_DATA pthunk, pthunk2;
    PIMAGE_IMPORT_BY_NAME pOrdinalName;
    int i;

    dwBase = (DWORD)GetModuleHandleA(NULL);

    pDosHeader = (PIMAGE_DOS_HEADER)dwBase;

    pNtHeaders = (PIMAGE_NT_HEADERS)(dwBase + pDosHeader->e_lfanew);

    pOptHeader = &(pNtHeaders->OptionalHeader);

    dwRVAImpTbl = pOptHeader->DataDirectory[1].VirtualAddress;

    dwSizeOfImpTbl = pOptHeader->DataDirectory[1].Size;

  	pImpTbl = (PIMAGE_IMPORT_DESCRIPTOR)(dwBase + dwRVAImpTbl);

    for (pImpDesc = pImpTbl; 
                     (DWORD)pImpDesc < ((DWORD)pImpTbl + dwSizeOfImpTbl); 
                     pImpDesc++) {
        printf("=====================================\n");
        printf("Import Desc Address: 0x%08x\n", pImpDesc);
        if (pImpDesc->Name == 0) {
           printf("Import Name: None\n");
           break;
        }
        printf("Import Name: %s\n", dwBase + pImpDesc->Name);
	    
        pthunk = (PIMAGE_THUNK_DATA) (dwBase + pImpDesc->OriginalFirstThunk);

        pthunk2 = (PIMAGE_THUNK_DATA) (dwBase + pImpDesc->FirstThunk);

        i = 0;
	    
        while (pthunk->u1.Function != 0) {
            printf("\tTHUNK_DATA[%d]: 0x%08x\n", i, pthunk->u1.AddressOfData);
            if (pthunk->u1.Ordinal & 0x80000000) {
	            printf("\t\tFunction Ord: 0x%04x\n", pthunk->u1.Ordinal | 0x7FFFFFFF);
                printf("\t\tFunction Name: <NULL> \n");
            } else {
		        pOrdinalName = (PIMAGE_IMPORT_BY_NAME)(dwBase + pthunk->u1.AddressOfData);
	            printf("\t\tFunction Ord: 0x%04x\n", pOrdinalName->Hint);
                printf("\t\tFunction Name: %s\n", &pOrdinalName->Name);
            }
            printf("\t\tAddress of Import Address: 0x%08x\n", pthunk2);
            printf("\t\tImport Address: 0x%08x\n", pthunk2->u1.Function);
            pthunk++;
            pthunk2++;
            i++;
        }
    }
    return 0;
}
