#include <stdio.h>//也可以包含 x.c 
int main(){
    int x=9;
    __asm call func;//call  eip+offset == 0xe8  offset ; indirect call ==0xff
    printf("%d",x);
    return 0;
    __asm{
          func :
               mov dword ptr [x] ,5
               ret //0xc3
          }
          
    
    }
