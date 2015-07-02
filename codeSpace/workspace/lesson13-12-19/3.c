int main(){
    
    return 0;
    
    __asm{
          mov eax , offset main
          sub eax ,0x1000
          ret
          
          }
    }
