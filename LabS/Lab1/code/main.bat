@echo off
cl /c main.c
cl /c sum1.c
link /dynamic:no main.obj sum1.obj
pause