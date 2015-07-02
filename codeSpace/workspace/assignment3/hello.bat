@echo off
cl /c hello.c
cl /c msg.c
link /dll msg.obj
editbin /rebase:base=0x20000000 msg.dll
link /dynamicbase:no hello.obj msg.lib
editbin /rebase:base=0x00400000 hello.exe
hello
@echo off