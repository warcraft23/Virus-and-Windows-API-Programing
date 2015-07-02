#include <windows.h>
#pragma comment(lib,"user32.lib")
int WINAPI WinMain(HMODULE hInstance, HMODULE hprev, LPSTR lpCmdLine, int nCmdShow)
{
    while (MessageBoxA(0,"hello", "hello.exe", MB_YESNO) 
        == IDYES) {}
    return 0;
}