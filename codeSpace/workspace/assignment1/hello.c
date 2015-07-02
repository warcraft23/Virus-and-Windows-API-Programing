#include <stdio.h>
int main(int argc,char *argv[]){
    //char *a="haha";
   // int addr=(int)a;
    //printf("%08x",addr);
    unsigned int addr=0;
    
    my_print("hello\n");   
    __asm{
          sub esp,4
          mov eax,[esp]
          mov dword ptr [addr],eax
          }
          
    printf("%08x\n",addr);
    
    
    
    printf("hello: %08x\n");
    
    
    
    //printf("outer %p\n",printf);
    printf("main: %p\n",main);
    printf("my_print: %p\n",my_print);
    printf("printf: %p\n",printf);
    
    system("PAUSE");
    
    return 0;
    }
