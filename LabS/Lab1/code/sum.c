/* sum.c */
#include <stdio.h>
int sum(int i) 
{
    __asm {
    // 计算并返回 1+2+…+i的值
    mov eax,0
comeBack:
    cmp dword ptr [i],0
    je finish
    add eax,dword ptr [i]
    dec dword ptr [i]
    jmp comeBack
finish:
    mov dword ptr [i],eax
    
    }
    return i;
}
int main()
{
    // 输出sum(100)的值
    int num=100;
    //int res;
    //res=sum(num);
    printf("%d\n",sum(num));
    
}
