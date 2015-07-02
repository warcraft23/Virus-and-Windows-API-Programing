@echo off
cl /c hello.c
link hello.obj
cl /c inject.c
link inject.obj
hello
@echo off