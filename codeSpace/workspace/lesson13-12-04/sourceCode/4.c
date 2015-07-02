#include <stdio.h>
int main(){
    
    int x;
    __asm{
          mov eax , 9
          mov dword ptr [x],eax
          //push eax
          }
    printf("%d\n",x);
    /*__asm{
          mov ebx,50
          push ebx
          }*/
    __asm{
          //pop eax
          mov dword ptr [x],eax
          }
    printf("%d\n",x);
    return 0; 
    }
