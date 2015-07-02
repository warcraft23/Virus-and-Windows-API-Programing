#include <stdio.h>
int main(){
    int x,y,z;
    x=1;
    y=2;
    __asm{
          mov eax, dword ptr [x]
          add eax, dword ptr [y]
          mov dword ptr [z],eax
          
          }
          printf("%d+%d=%d",x,y,z);
          return 0;
    }
