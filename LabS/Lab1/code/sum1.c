int sum(int i) 
{
    __asm {
    // 计算并返回 1+2+…+i的值
    mov eax,0 //eax用于保存返回值 
    mov ecx ,dword ptr [i] 
comeBack:
    add eax,ecx
    loop comeBack
    //mov [i] , eax
   
    }
    //return i;
}
