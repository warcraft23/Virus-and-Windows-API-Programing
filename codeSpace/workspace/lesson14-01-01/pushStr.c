#include <stdio.h>//不懂
int main(){
__asm{
	call _func//push eip并跳转
	_emit 'h'
	_emit '0'
	_emit 0x0
_func:
	call printf//把上面push的eip当成参数传入printf
	add esp,0x4

}

}