#include <stdio.h>
struct ctx {
  int eip, esp, ebx, ebp;
} M, A, B;

__declspec(naked) void swtch(struct ctx *from, struct ctx *to)
{
 __asm{
        mov eax, [esp+4] //将M的地址值赋给ax  eax=&from
        pop dword ptr [eax] //from.eip=ret
        mov [eax+4], esp //from.esp=esp
        mov [eax+8], ebx//from.ebx=ebx
        mov [eax+12], ebp//from.ebp=ebp
        mov eax, [esp+8] //eax=&lastFrom
        mov ecx, [esp+4] //ecx=&to
        mov ebp, [ecx+12] //ebp=to.ebp
        mov ebx, [ecx+8] //ebx=to.ebx
        mov esp, [ecx+4] //esp=to.esp
        push [ecx] //push to.eip
        ret //esp=esp+4;return to to.eip
    }
}

void A_proc()
{
    printf("A: 1\n");
    swtch(&A, &B); //从函数A切换到函数B
    printf("A: 2\n");
    swtch(&A, &B);
    printf("A: 3\n");
    swtch(&A, &M);
}

void B_proc()
{
    printf("B: 1\n");
    swtch(&B, &A); //从函数B切换到函数A
    printf("B: 2\n");
    swtch(&B, &A);
}

int main()
{
  int Astack[1024];
  int Bstack[1024];
  A.eip = (int)A_proc;//A过程的初始地址 
  A.esp = (int)(&Astack[1023]);//A的栈顶 
  B.eip = (int)B_proc;//B过程的初始地址 
  B.esp = (int)(&Bstack[1023]);//B的栈顶 
  swtch(&M, &A);//从main函数切换到A函数 
  return 0;
}
