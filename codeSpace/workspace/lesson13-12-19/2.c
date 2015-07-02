#include <windows.h>
#include <stdio.h>
char *code_addr="\x33\xc0\xc3";

int main(){
    int old;
    BOOL r;
    printf("code addr:0x%08x\n",code_addr);
    
    r=VirtualProtect(code_addr,3,0x40,&old);//¸ÄÈ¨ÏÞ 
    if(!r){
           printf("Error!");
           return -1;
           }
    __asm{
          call code_addr
          }
    return 0;
    
    
    }
