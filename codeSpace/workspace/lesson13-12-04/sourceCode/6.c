#include <stdio.h>
int main(){
    int x=9,y=0;
    __asm
    {
         mov eax,dword ptr [x]
         _emit 0x89 //每次一个byte 
         _emit 0xc1 //mov ecx,eax
         mov dword ptr [y],ecx
         }
    printf("%d",y);
    return 0;
    }
