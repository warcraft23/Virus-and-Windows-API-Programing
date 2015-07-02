/* main.c */
//#include <stdio.h>
extern int sum(int);

int main(int argc, char *argv[]) 
{
  int i;
  //int res;
  char *sa="%d\n";
  if (argc < 2) {
    printf("usage: main number\n");
    return -1;
  }
  i = atoi(argv[1]);
  //printf("%d\n",i);
    __asm {
        // 计算并输出sum(i)的值
        mov edx,dword ptr [i]
        push edx
        call sum
        //pop dword ptr [res] 
        //lea edx , ch
        //pop [res]
        //push [res]
        push eax
        push sa
        call printf
        ////add esp,8
        //mov dword ptr [res],eax
    }
    
    
   
    //printf("%d\n",res);
}
