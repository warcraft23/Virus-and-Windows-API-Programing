#include<windows.h>

#pragma comment(lib, "user32.lib")

char *s = "Hello world!";

int main()
{

	while (MessageBoxA(0, s, "hello", MB_YESNO) == IDYES)
		;
	return 0;
}
