#include <windows.h>
#include <stdio.h>

int main()
{
   char *str ="LoadLibraryA: 0x%08x\n";
   __asm{
     push dword ptr[LoadLibraryA]
     push str
     call printf
     add  esp, 0x8
   }
   
   return 0;
   
   __asm {
     
       push 0xBBAADDCC
       mov  eax, 0xAABBCCDD
       call eax
       ret  
     
   }
}