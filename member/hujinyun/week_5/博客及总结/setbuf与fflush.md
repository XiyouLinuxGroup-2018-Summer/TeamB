###setbuf
>程序输出有两种方式：一种是即时处理方式，另一种是先暂存起来，然后再大块写入的方式，前者往往造成较高的系统负担。因此，c语言实现通常都允许程序员进行实际的写操作之前控制产生的输出数据量。

这种控制能力一般是通过库函数setbuf实现的。如果buf是一个大小适当的字符数组，那么：

setbuf(stdout，buf);

语句将通知输入/输出库，所有写入到stdout的输出都应该使用buf作为输出缓冲区，直到buf缓冲区被填满或者程序员直接调用fflush（注：对于由写操作打开的文件，调用fflush将导致输出缓冲区的内容被实际地写入该文件），buf缓冲区中的内容才实际写入到stdout中。缓冲区的大小由系统头文件< stdio.h >中的BUFSIZ定义。

---
###函数声明
```
void setbuf(FILE *stream, char *buffer)
```
###参数
* stream--这是指向FILE对象的指针,该FILE对象标识了一个打开的流
* buffer--这是分配给用户的缓冲,他的长度至少为BUFSIZ字节,BUFSIZ是一个宏常量,表示数组的长度.
###返回值
该函数不返回任何值
###实例
```
#include<stdio.h>
int main()
{
    char buf[BUFSIZ];
    setbuf(stdout, buf);
    puts("This is my test");
    
    fflush(stdout);
    return 0;
}
```
程序在即将输出的时候,发送输出到STDOUT,否则将它缓冲输出

---
###fflush
####描述
c库函数 int fflush(FILE *stream)刷新流stream的输出缓冲区
####声明
下面是fflush()函数的声明
```
int fflush(FILE *stream)
```
####参数
* stream---这是指向FILE对象的指针,该FILE对象指定了一个缓冲流
####返回值
如果成功,该函数返回零值.如果发生错误,则返回EOF,且设置错误标识符
####实例
下面的实例演示了fflush()函数的使用方法
```
#include<stdio.h>
#include<string.h>
#include<unistd.h>

int main()
{
    char buff[1024];
    memset(buff, '\0', sizeof( buff ));
    fprintf(stdout,"启用全缓冲\n");
    setvbuf(stdout,buff,_IOFBF, 1024);
    fprintf(stdout,"这里是 okokme\n");
    fprintf(stdout,"该输出将保存到 buff\n");
    fflush(stdout);
    
    fprintf(stdout,"这将在编程时出现\n");
    fprintf(stdout,"最后休眠5秒中\n");
    sleep(5);
    return 0;
    
}
```
编译运行后,将产生以下结果

5秒后会出现

在这里,程序把缓冲输出保存到buff,直到首次调用fflush()为止,
然后开始流缓冲,最后休眠5秒钟.他会在程序结束之前,发送剩余的输出到STDOUT

fflush用来强制刷新缓冲区数据,如果需要在每次i/o操作前后,不希望缓冲中存在历史数据或者不期望的数据或者为了清除缓存等等 的时候使用.


---
###setbuf与fflush为什么有时候要一起使用呢
为了将你想写的语句完整写入缓冲区,再一并输出或写入文件,既保证了数据的完整又没有乱七八糟缓冲区残留.

tips:写完文件之后最好调用fflush,否则有可能
fclose之后文件的最后有一部分没有写入完整.