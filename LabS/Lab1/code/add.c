#include <stdio.h>
int add(int x, int y) 
{
    //int z;
    __asm {
        //返回x+y的值
        mov eax,dword ptr [y]
        add eax,dword ptr [x]
        //mov dword ptr [z],eax
    }
    //return z;
}
int main()
{
    //输出 add(2,4)的值
    printf("%d\n",add(2,4));
}
