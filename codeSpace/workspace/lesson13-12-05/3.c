#include <stdio.h>
__declspec(naked) int __stdcall add(int x,int y){ 
                  
                  __asm{
                        mov eax,[esp+4]
                        add eax,[esp+8]
                        ret  8               
                  }
                  }
int main(int argc,char *argv[]){
    printf("%d\n",add(5,6));
    return 0;
    }
