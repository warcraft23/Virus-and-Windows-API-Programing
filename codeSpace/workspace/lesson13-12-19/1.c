#include <stdio.h>
int y=10;
char *s="%d\n";
__declspec(naked) int main()
{

 __asm{
       mov eax ,[y]
       call xxxx
       
       jmp finish
 xxxx:
       xor eax,eax
       ret
       
 finish:
        mov [y],eax
        
       }
      
      printf("%d\n",y);
      
      __asm{
            
            
            }
       
}
