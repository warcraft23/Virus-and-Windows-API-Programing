#include <stdio.h>
int main(){
    int x=1;
    unsigned int y=0;
    __asm{
          lea eax ,dword ptr [x]
          mov dword ptr [y] ,eax
          
          }
    
    printf("the addr of x:%p\n",y);
    return 0;
    }
