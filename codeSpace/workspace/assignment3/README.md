hello.bat产生的是第一个作业的内容

rwproc.exe是第二个作业的内容

PBYTE pRemote = (PBYTE)0x00401000;//用于修改0x00401000 位置的内容

修改后从0x00401000开始的11个单元格内容被修改为 I'm hacked\0 的16进制数

PBYTE pRemote = (PBYTE)0x00409af9;//用于修改弹出框的内容

PBYTE pRemote = (PBYTE)0x6b0c8000;//用于修改标题栏的内容

作业二的操作方式是，修改rwproc.c中的pRemote值，让其指向不同的进程空间，修改对应位置的内容，从而改变程序的表现形式

cl rwproc.c

hello

获取hello的pid后

rwproc <pid of hello>即可

获取地址值得方法如下

开启ollydbg

运行程序，在while处设断点，跑到while处，

出提示框，看压入参数地址，然后找对应 内存区域的值，

找到对应区域后，从偏移地址，找到字符串buf中helloworld所在的地址，获得真实地址，然后修改此地址即可 