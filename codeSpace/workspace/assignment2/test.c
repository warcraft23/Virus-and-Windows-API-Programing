#include <stdio.h>
int main (int argc,char *argv[]){
    int n=atoi(argv[1]);
    int r=0;
    r=fib(n);
    printf("%d\n",r);
    return 0;
    }
    
int fib(int num){
    //int i,j;
    //i=0;
    //j=0;
    
    int res=0;
    if(num==1){
               return 1;
               }
    if(num==2)
    {
              return 2;
              }
    
    res=fib(num-1)+fib(num-2);
    
    return res;
    
    }
