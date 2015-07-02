2014-01-14 20:30:10  0xAB(26271544)
给大家细化下考试范围

1. 9个寄存器都是干嘛用的
//  EAX：累加器 Accumulator
//  EBX：基址寄存器 Base
//  ECX：计数寄存器 Count
//  EDX：数据寄存器 Data
//  ESP：堆栈指针寄存器 Stack Pointer
//  EBP:基址指针寄存器 Base Pointer
//  ESI：源变址寄存器 Source Index
//  EDI：目的变址寄存器 Destination Inde
  
  EAX, accumulator:累加寄存器，用于存储计算中间结果的缓存
  EBX, base：基址寄存器
  ECX, counter：计数寄存器，可以用来计数循环次数
  EDX, data/general：数据寄存器
  上面四个寄存器在实际中功能比较通用，但是又有其默认的一些功能；
  ESI, source index for string operation：源变址寄存器：存储源操作数的指针
  EDI, destination index for string operation：目的变址寄存器：存储目标操作数的指针
  ESP, stack pointer for top address of the stack：堆栈指针寄存器，指向栈顶
  EBP, stack base pointer current stack frame：基址指针寄存器：指向当前环境的栈基址
  上面八个是通用寄存器，EIP不是通用寄存器；
  EIP, instruction pointer：指令寄存器：指向下一条要执行的指令

2. 哪些寄存器是caller-saved registers 哪些是callee-saved registers

    根据惯例，寄存器eax、edx和ecx为调用者保存寄存器（caller-saved registers），
当函数P调用Q时，Q可以覆盖这些寄存器，而不会破坏任何P所需的数据。
寄存器ebx、esi和edi为被调用者保存寄存器（callee-saved registers），
即Q在覆盖这些寄存器的值时，必须先把它们保存到栈中，并在返回前恢复它们，
因为P可能会用到这些值。此外，根据惯例，必须保持寄存器ebp和esp，即函数返回后，
这两个寄存器必须恢复到调用前的值，换句话说，必须恢复调用者的栈帧。

3. 实验中常用系统API的作用是什么，比如LoadLibraryA, GetProcAddress, OpenProcess, GetModuleHandleA, WriteProcessMemory .. 等等

LoadLibraryA:载入指定名称的动态链接库，然后返回载入地址；
GetProcAddress：函数检索指定的动态链接库(DLL)中的输出库函数地址，然后返回该地址；
OpenProcess：函数用来打开一个已存在的进程对象，并返回进程的句柄；
GetModuleHandleA：用来获取一个应用程序或动态链接库的模块句柄；
WriteProcessMemory：此API能够向指定进程的内存空间写入数据，在参数中可以指定目标进程的句柄，目标地址，待写入数据的缓存首地址，写入数据大小，实际写入数据的大小；
VirtualAllocEx：在指定进程的地址空间内分配具有一定权限和大小的内存，参数中可以指定目标进程句柄，分配内存的大小，分配内存的读写可执行属性等等。若成功则返回该新分配内存的首地址。
CreateRemoteThread：在远程进程中创建一个线程。可以指定目标进程的句柄和远程线程的代码首地址等，执行成功返回线程句柄；
WaitForSingleObject：等待指定句柄的线程执行，参数中可以指定时间；
GetExitCodeThread：用于取出目标线程句柄的返回值，并存放在第二个参数的地址里。
GetCurrentProcessId：返回当前进程PID；
ReadProcessMemory：读入目标进程内指定地址和指定大小的数据。参数里可以指定目标进程的句柄，目标地址，本地存放读入数据的地址，读入数据的大小和实际读入的数据大小；
VirtualProtect：改变指定起始地址处指定大小的数据的权限，参数中可以指定目标起始地址，改变权限的数据大小，新的权限类型等等；
VirtualProtectEx：同上，只不过改变的是远程进程数据的权限，第一个参数变为目标进程的句柄，其他参数一致；

4. 常见指令的用法以及含义, mov, jmp, call, push, pop, lea, add, sub, cmp, ret, .. 

5. PE结构头部的关键数据域的名称和含义, 比如AOEP, ImageBase, NumberOfSections, 等等!

NumberOfSections：sections的数目，windows指定最大值为96；
AddressOfEntryPoint：即AOEP，当可执行文件被加载进内存时其入口点的RVA。对于一般程序镜像来说，它就是启动地址。
ImageBase：镜像基址，即加载进内存时镜像的第1个字节的首选地址。
Subsystem：子系统类型，即运行此镜像所需的子系统。
Export Table：导出表的地址和大小；
Import Table：导入目录表的地址和大小。
IAT：导入地址表的地址和大小。

6. 什么是calling convention，常见的calling convention都有哪些，它们之间有什么区别，都有什么用处

//***函数的调用方与被调用方对于函数如何调用须要一个明确的约定，这样的约定就叫作调用惯例。
一个调用惯例一般会规定如下几方面的内容：
//***函数参数的传递顺序及方式
函数参数的传递有多种方式，常见的是通过栈传递。函数的调用方将参数压入栈中，
函数自己再从栈中取出参数。对于有多个参数的函数，
调用惯例需要约定函数调用方将参数压入栈的顺序：
是从左到右，还是从右到左。还有些调用惯例也支持使用寄存器来传递参数。
//***栈的维护方式
在参数被压入栈后，函数体被调用执行，此后需要将被压入栈的参数再全部弹出，以使得栈在函数调用前后保持一致。
这个弹出工作可以由调用方完成，也可以由被调用函数来完成。
//***名字修饰的策略
为了链接的时候能够对调用惯例进行区分，调用管理要对函数名进行修饰。不同的调用惯例也有不同的修饰策略。
//***下表列出几个常见的调用惯例：
调用惯例*出栈方*参数传递*名字修饰
cdecl	调用方	从右至左的顺序压参数入栈	下划线+函数名
stdcall	函数自身	从右至左的顺序压参数入栈	下划线+函数名+@+参数的字节数，如函数int func(int a, double b)的修饰名是_func@12
fastcall	函数自身	头两个DWORD(4字节)类型或者占更少字节的参数被放入寄存器，其他剩下的参数按从右到左的顺序入栈	@+函数名+@+参数的字节数
不少编译器还提供一种称为naked call的调用惯例，这种调用惯例特点是编译器不产生任何保存寄存器的代码，故称为naked call，用于一些特殊场合。

7. PE结构中RVA的含义和作用是什么

英文全称Relatively Virtual Address。偏移（又称“相对虚拟地址”），即相对镜像基址的偏移。在PE结构中，标记地址采用的是RVA的方式，即PE数据结构中
给出的地址只是相对Imagebase的偏移，即RVA，需要用Imagebase加上目标RVA才能得到目标数据的真实地址。

8. 如何写一个code 注入器, (injector)，分析总结实验中的代码

首先要使用OpenProcess函数获得目标进程的句柄，然后使用该句柄调用VirtualAllocEx函数在远程进程的地址空间分配注入代码的地址空间，并把地址空间
设为可执行，然后使用WriteProcessMemory函数将欲注入的代码写入远程地址空间，最后调用CreateRemoteThread函数，传入已写入可执行代码的首地址就可以实现
创建远程线程执行注入代码了。

9. 实验三, IAT hooking的原理及过程, 什么是IAT, IAT有什么作用，如何找到IAT，IAT和Import Table的关系

首先查找导入目录表的首地址，然后依次读入导入目录表的表项，每个表项代表一个dll的导入情况。导入目录表的每个表项中都包含了导入表IT和导入地址表IAT，
其中导入查找表中的每一项都对应着导入地址表的每一项，因此可以遍历导入查找表中的字符串，与目标函数的名称进行比对，同时使导入查找表和导入地址表的
指针同步移动，直到找到目标函数，此时指向导入地址表项的那个指针即指向存放“目标函数入口地址”的内存地址。IAT hooking即可以更改此目标函数入口地址，使其
指向我们自己写的code，然后当该进程调用这个函数的时候，自动执行我们注入的代码。

IAT即导入地址表，里面存储的是各导入函数的真实内存地址。查找IAT可以读取PE头部DataDirectory中对应表项的IAT部分。

Import table即导入目录表，里面每个表项是一个dll的导入信息，每个导入目录表的表项中含有一个导入查找表和IAT的RVA，当程序被加载进内存空间以后，
IAT里存储的就是对应dll导入的函数的真实入口地址。


10. 用汇编语言写一个函数的基本结构是什么, 什么是函数的prolog, epilog， 它们的作用是什么，和calling convention 有什么关系，

Push ebp //把ebp压入栈中
Mov ebp ,esp//让ebp指向目前的栈顶，此时栈顶是old ebp
Sub esp,***//开辟临时空间
Push **,//保存各种寄存器
Push * //压入参数
Push * //压入参数
Call function //调用函数（包括两步：把下一条指令地址压入栈中；跳转到函数体执行）
Pop *** //恢复保存过得寄存器
Mov esp,ebp //恢复esp 同时回收局部变量空间
Pop ebp //从栈中恢复保存的ebp的值
Ret //从栈中取得返回地址，并跳转到该位置

prolog和epilog都是编译器产生的，自动加在汇编指令的头部和尾部的代码。prolog主要作用是设置栈帧的环境，为本地变量分配内存空间，保存callee-saved寄存器。
epilog的作用是释放栈帧空间，恢复callee-saved寄存器现场。

当使用naked调用约定的时候，就表示所有的prolog和epilog代码编译器都不自动生成，此时需要按照call的各种类型如stdcall等手动添加prolog和epilog应该完成功能的代码

11. 什么是动态链接，如何编译产生一个DLL, 在运行过程中，主程序如何和DLL建立起通道，发生调用关系

动态链接就是把程序调用的模块与主程序分开存储，当程序装入内存时，才将各个模块与主程序链接起来。

编译产生dll的过程：首先编译源文件得到obj文件，然后使用link/dll命令链接obj文件得到dll文件，在命令的后面可以加上/EXPORT选项来指定导出的函数。

主程序在没有装入内存时，程序中调用dll中函数的地方都是用全零代替，装入内存过程中，装入器装入dll并生成导出表，同时将对应的导入表
修改为函数的真实地址。然后装入器将IAT中导入函数的真实地址填入主程序中，来使程序正常运行。

12. 实验四，把一段代码插入到一个PE文件中，都需要哪些步骤

修改NumberOfSections字段；
添加进新的Section段，并获得其地址和大小等属性；
加入新的SectionTable表项，将其各个字段调整为需要添加的section的属性；

13. 什么是窗口消息，什么是窗口过程，窗口过程的作用是什么，如何修改窗口过程。

窗口消息是系统发给应用程序的一系列操作消息，比如鼠标移动，键盘输入等等。
窗口过程可以处理窗口消息，做出相应的响应。
修改窗口过程使用SetWindowLongA函数，其中需要指定窗口句柄，使用GWL_WNDPROC参数和传入新窗口过程的入口地址。

14. 静态链接和动态链接的区别和联系

静态链接在编译的时候直接填入调用函数的地址，而动态链接在程序没有装入的时候用零代替调用函数的地址，在装入过程中动态的填入调用模块的真实地址。


15. over 没有了，以上覆盖80%考试内容 希望大家确保这些知识点都懂

希望大家本着走过路过的心态，好好复习1-14，把15点完全忘掉  
