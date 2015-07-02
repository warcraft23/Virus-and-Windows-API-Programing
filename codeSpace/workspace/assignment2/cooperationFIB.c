#include <stdio.h>
struct ctx {
  int eip, esp, ebx, ebp;
} M, A, B;

int n;
int r=0;

__declspec(naked) void swtch(struct ctx *from, struct ctx *to)
{
 __asm{
        mov eax, [esp+4] //将M的地址值赋给ax  eax=&from
        pop dword ptr [eax] //from.eip=ret
        mov [eax+4], esp //from.esp=esp
        mov [eax+8], ebx//from.ebx=ebx
        mov [eax+12], ebp//from.ebp=ebp
        
        //mov eax, [esp+8] //eax=&lastFrom
        mov eax , [esp] //eax=&from
        
        mov ecx, [esp+4] //ecx=&to
        mov ebp, [ecx+12] //ebp=to.ebp
        mov ebx, [ecx+8] //ebx=to.ebx
        mov esp, [ecx+4] //esp=to.esp 栈顶地址改变，到了另一块区域 
        push [ecx] //push to.eip
        ret //esp=esp+4;return to to.eip
    }
}

void A_proc()
{ 
    //while(count<n){             
//                   r=B.ebx;//将B的计算结果取出 
//                   __asm{
//                         add ebx,dword ptr [r]//将A的计算结果和B的计算结果相加 
//                         }
//                   count++;
//                   swtch(&A,&B);//将A的计算结果存入结构体A中，并读出结构体B中的计算结果。并切换 
//                   }
//                   swtch(&A,&M); //到达计数次数跳回至M进程 
 __asm{
             
        WHILE:
              
              cmp edx , 0
              je Final
              //push ebp
              mov eax , [eax+8]//r=B.ebx
              //mov eax , [eax+8]//r=B.ebx
              mov dword ptr [r] , eax//r=B.ebx 
              //pop eax
              //pop ebp
              
              add ebx,dword ptr [r]
              dec edx
//            xor eax,eax
//            }
//            //printf("A:%d\n",r);
//            swtch(&A,&B);
//            //printf("A:%d\n",r);
// __asm{
              lea eax,dword ptr [B]
              push eax
              push ecx //ecx=&from
              call swtch
              add esp,8
         
              jmp WHILE
        Final:
              mov dword ptr [r],ebx
              lea eax , dword ptr [M]
              lea ecx , dword ptr [A]
              push eax
              push ecx
              call swtch 
        }
        //swtch(&A,&M);


}

void B_proc()
{
    /*
    printf("B: 1\n");
    swtch(&B, &A); //从函数B切换到函数A
    printf("B: 2\n");
    swtch(&B, &A);
    */ 
    //while(count<n){
//                   r=A.ebx;//将A的计算结果取出
//                   __asm{
//                         add ebx,dword ptr [r]//将B的计算结果和A的计算结果相加 
//                         }
//                   count++;
//                   swtch(&B,&A);//将B的计算结果存入结构体B中，并读出结构体A中的计算结果。并切换
//                   }
//                   swtch(&B,&M); //到达计数次数跳回至M进程 
        
        __asm{
              
        WHILE:
              cmp edx , 0
              je Final
              //push edx
              //push eax
             // mov eax , [eax+8]//r=A.ebx
              mov eax , [eax+8]//r=A.ebx
              mov dword ptr [r] , eax//r=A.ebx
              
              //pop eax
              //pop edx
              
              add ebx,dword ptr [r]
              dec edx
              //xor eax,eax
             // }
//           //printf("B:%d\n",r);
//           swtch(&B,&A);
//           //printf("B:%d\n",r);
//           __asm{
             lea eax,dword ptr [A]
             push eax
             push ecx
             call swtch
             add esp,8
             jmp WHILE
        Final:
              mov dword ptr [r],ebx
              lea eax , dword ptr [M]
              lea ecx , dword ptr [B]
              push eax
              push ecx
              call swtch 
              }
        //swtch(&B,&M);
              
              
                         
}

int main(int argc,char *argv[])
{
  int Astack[1024];
  int Bstack[1024];
  
  
 if(argc<2){
           printf("Usage: %s number(<40)\n",argv[0]);
           return -1;
           }
   
   n = atoi(argv[1]);
  //n=3;
  
  A.eip = (int)A_proc;//A过程的初始地址 
  A.esp = (int)(&Astack[1023]);//A的栈顶
  A.ebx = 1; 
  B.eip = (int)B_proc;//B过程的初始地址 
  B.esp = (int)(&Bstack[1023]);//B的栈顶 
  B.ebx = 1;
  __asm{
        mov ebx , 1
        mov edx , dword ptr [n]
        }
  //printf("fib(%d)=%d\n",n,r);
  swtch(&M, &A);//从main函数切换到A函数 
  //printf("fib(%d)=%d\n",n,r);
  /*__asm{
        mov dword ptr [r] , ebx
        }*/
  //r=tmp;
  printf("fib(%d)=%d\n",n,r);
  return 0;
}
