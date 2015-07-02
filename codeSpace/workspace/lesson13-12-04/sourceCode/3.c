#include <stdio.h>
int main(){
    int x=9;
    __asm{
          jmp label
          }
    x=6;
    __asm{
          label:
                nop
          }
    printf("%d",x);
    return 0;
    
    }
