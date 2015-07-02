#include <windows.h>
typedef int (* MYPROC)(char *s);
int main(){
//         HMODULE hmod = LoadLibraryA("msg.dll");
//         MYPROC proc =(MYPROC)GetProcAddress(hmod,"ShowMsg");
//         proc("hello\n");
//         FreeLibrary(hmod);
//         return 0 
      char *s="msg.dll";
      char *s1="ShowMsg";
      char *s2="hello\n";

      
      __asm{
            push dword ptr[s]
            call dword ptr [LoadLibraryA] 
            mov ebx,eax
            
            push dword ptr[s1]
            push eax
            call dword ptr [GetProcAddress]
            
            push dword ptr [s2]
            call eax
            add esp,4
            
            //mov ebx, dword ptr [hmod]
            push ebx
            call dword ptr [FreeLibrary]
            
            
            
            }
            return 0;
    }
