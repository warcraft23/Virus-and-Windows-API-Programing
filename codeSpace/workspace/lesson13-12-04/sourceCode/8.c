#include <stdio.h>
int i=0;
void func(){
     x=5;
     }
     
int main(){
    __asm{
          mov eax,func
          _emit 0xFF
          _emit 0xD0 //call eax (func)
          }
    printf("%d",x);
    }
