int ShowMsg();
int main(){
    char *s="hw";
    ShowMsg(s);
    return 0;
    }


//cmd:>cl /c msg.c
//     link /dll msg.obj /export:ShowMsg
//     cl /c main.c
//     link main.obj msg.lib    
//     generate main.exe
//     msg.lib is like a table from main.obj to msg.dll
//     code segement always starts from 0xXXXX0000
//     the size of PEheader is 1000
//     so the text segement always start from 0xXXXX1000
//     

//cmd:>cl /c msg.c
//     link /dll msg.obj /export:ShowMsg >> link /dll msg.obj /export:ShowMsg user32.lib   
