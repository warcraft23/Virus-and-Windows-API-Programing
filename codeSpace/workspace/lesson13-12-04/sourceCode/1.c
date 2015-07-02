#include <stdio.h>
int main(){
     char *x="xx";
     printf("hello\n");
    
     __asm{
           push x
           call printf
           add esp,4
           }
     return 0;
     }
