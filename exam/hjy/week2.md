C语言
1.根据优先级判断
...

linux进程操作简答
1.最终输出6次"helloworld?"
最终产生共5个子进程
父进程,子进程,两个孙子进程,一个重孙进程.
2.在使用fork或vfork创建子进程后,子进程通常会调用exec系列函数来执行另一个程序.系统调用exec用于执行另一个可执行程序以代替当前进程的映像.
包括六个函数:execv,execve,execl,execle,execvp,execlp
成功不会返回,失败返回-1
1.第一个参数:查找方式.前四个函数查找方式都是完整的目录文件路径,后两个以p结尾的函数可以只给出文件名,系统就会自动从环境变量所指出的路径进行查找.
2.第二个参数:传递方式,这里参数传递方式是以函数的第五个字母来区分的.字母为"l(list)"表示逐个列举的方式,字母位"v(vertor)"表示将所有参数整体构造成指针数组传递,然后将数组的首地址当参数传递给它,数组的最后一个指针要求是NULL.
3.第三个参数:环境变量exec函数族使用了系统默认的环境变量,也可以传入指的环境变量.以"e(environment)"结尾的函数就可以在第三个参数envp[]中指定当前进程所使用的环境变量替换掉该进程所继承额环境变量.
函数原型:
```
 #include <unistd.h>

int execl(const char *path, const char *arg, ...)

int execv(const char *path, char *const argv[])

int execle(const char *path, const char *arg, ..., char *const envp[])

int execve(const char *path, char *const argv[], char *const envp[])

int execlp(const char *file, const char *arg, ...)

int execvp(const char *file, char *const argv[])
```
3.
执行后的结果为
`
Hello,my pid is 5580
I was forked! 5581
5581 was forked!
`
test | cat
`
Hello,my pid is 5590
I was forked! 5592
Hello,my pid is 5590
5592 was forked!
`
父进程fork一个子进程,子进程输出I was forked后return 0结束,而父进程在等待,随后cat调用重定向,子进程结束后父进程部分重新判断将父进程部分全部打印输出.
