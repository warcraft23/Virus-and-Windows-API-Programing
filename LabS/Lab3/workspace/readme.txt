test.c是第一个和第二个步骤的代码
hello.c是第三个和第四个步骤要被攻击的代码
inj.c是注入器，若是要自删除什么可以遵从上个实验的内容
编译指令都是 
1.2.
cl /c test.c
link /dynamicbase:no test.obj

3.4.
cl /c hello.c
link /dynamicbase:no hello.obj
cl /c inj.c
link /dynamicbase:no inj.obj