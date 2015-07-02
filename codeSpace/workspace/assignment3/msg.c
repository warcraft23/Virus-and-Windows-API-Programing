#include <windows.h>
#pragma comment(linker,"/export:_myprint")
#pragma comment(lib,"user32.lib")

//int __stdcall MessageBoxA(
//    int hwnd,
//    char *txt,
//    char *cap,
//    int ut
//    );
    
int myprint(char* s){ //说明该函数将被导入 
    int res=0;
    res=MessageBoxA(NULL,
                    s,
                    "Message",
                    MB_OKCANCEL);
     if(res==6){
                return 1;
                }
     else if (res==2){
          return 0;
          }
     else{
          return -1;
          }
    
     }
