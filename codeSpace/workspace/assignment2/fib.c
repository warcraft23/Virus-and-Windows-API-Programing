#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
    int n,r=0;
    if(argc<2){
               printf("Usage: %s number(<40)\n",argv[0]);
               return -1;
               }
   
    n = atoi(argv[1]);
    //printf("%d %d\n",n,r);
    //printf("fib(%d) = %d\n", n, r);
    __asm{
          mov ebx,dword ptr [n] //用于计数 
          mov edx,0      //结果存储的地方
          call fib       //调用子过程 
          jmp FINISH
          
          fib:
              cmp ebx,0 //计数值为0则跳转至LO赋值dx为1 
              je L0
              cmp ebx,1 //计数值为1则跳转至L1赋值dx为1
              je L1
              dec ebx
              push ebx //计数值会在过程中被修改，所以压栈保存
              call fib
              pop ebx
              mov eax,edx //将结果保存于ax便于累加 
              push eax //将累加器中的值压栈，防止在子过程中被修改
              dec ebx
              push ebx
              call fib
              pop ebx 
              pop eax
              add eax,edx
              xchg edx,eax
              jmp fin
          L0:
              mov edx,1
              jmp fin
          L1:
              mov edx,1
          fin:
              ret           
          FINISH:
                 mov dword ptr [r],edx
          }
    printf("fib(%d) = %d\n", n, r);
    return 0;
    }
