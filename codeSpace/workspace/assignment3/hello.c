#include <windows.h>

int myprint(char *s);

char *s = "Hello world!";
char buf[100];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int pid = 0;
    pid = GetCurrentProcessId();
    sprintf(buf, "base=0x%08x\npid=%d\n%s", hInstance, pid, s);
    while (myprint(buf))
      ;
    return 0;
}

//编译过程：
//1.cl /c hello.c
//2.cl /c msg.c
//3.link /dll msg.obj
//4.editbin /rebase:base=0x20000000 msg.dll
//5.link hello.obj msg.obj
//6.editbin /rebase:base=0x00400000 hello.exe

//作业2：
//link /dynamicbase:no hello.obj msg.lib 
//hello
//rwproc <pid of hello>
