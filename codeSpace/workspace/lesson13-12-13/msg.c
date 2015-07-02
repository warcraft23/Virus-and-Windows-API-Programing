//msg1:
//#include <stdio.h>
//void ShowMsg(char *s){
//     printf(s);
//     return;
//     }

//msg2

//#include <windows.h>
#pragma comment(linker,"/export:_ShowMsg")
#pragma comment(lib,"user32.lib")
//__declspec(dllexport) 

int __stdcall MessageBoxA(
    int hwnd,
    char *txt,
    char *cap,
    int ut
    );
    
void ShowMsg(char* s){ //说明该函数将被导入 
     MessageBoxA(0x0,s,"Msg",0x0);
     return;
     }
