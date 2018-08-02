# 守护进程




标签： Linux系统编程之进程

---

Daemon(守护进程)是运行在后台的一种特殊进程.没有控制终端与之相连.
特点:独立于终端控制,周期性地执行某种任务
守护进程启动方式: 从启动脚本/etc/rc.d; 由作业规划crond启动; 又用户终端(通常是shell)执行

编写守护进程要点:
* 让程序在后台执行.方法是fork产生一个子进程,然后使得父进程退出.
```
pid_t pid;
pid = fork();/*产生子进程*/

if(pid > 0)
exit(0); /*结束父进程使得子进程成为后台进程*/
else if(pid < 0)
exit(-1);
```
* 调用setsid创建一个新对话框.控制终端,登录会话和进程组通常是从父进程继承下来的.守护进程要摆脱他们,不受他们的影响,其方法是调用setsid使进程称为一个会话组长.(**setsid可以创建一个新的对话**)
(当进程是会话组长时调用setsid失败,但是第一点已经保证进程不是会话组长)
会话(session)是一个或多个进程组的集合。
开始于用户登录
终止与用户退出
此期间所有进程都属于这个会话期
进程通过getsid()获得回话id
```
 1 #include<stdio.h>

 2 #include<unistd.h>

 3 #include<sys/types.h>

 4

 5 int main() {

 6     pid_t pid;

 7

 8     if ((pid=fork())<0) {

 9         printf("fork error!");

10         exit(1);

11     }else if (pid==0) {

12         printf("The child process PID is %d.\n",getpid());

13         printf("The Group ID of child is %d.\n",getpgid(0));

14         printf("The Session ID of child is %d.\n",getsid(0));

15         sleep(10);

16         setsid(); // 子进程非组长进程，故其成为新会话首进程，且成为组长进程。该进程组id即为会话进程

17         printf("Changed:\n");

18         printf("The child process PID is %d.\n",getpid());

19         printf("The Group ID of child is %d.\n",getpgid(0));

20         printf("The Session ID of child is %d.\n",getsid(0));

21         sleep(20);

22         exit(0);

23     }
    return 0;

 }
```

![进程会话][1]

  pid_t setsid(void);

该调用进程是组长进程，则出错返回

该调用进程不是组长进程，则创建一个新会话

该进程变成新会话首进程(session header)

该进程成为一个新进程组的组长进程。

该进程没有控制终端，如果之前有，则会被中断

 组长进程不能成为新会话首进程，新会话首进程必定会成为组长进程
 
 <mark>在子进程中调用setsid()后，子进程成为新会话首进程，且成为一个组长进程，其进程组id等于会话id,进程可以通过setpgid()来加入已存在的进程组或创建一个新的进程组。</mark>
 
    ```
    /*建立新进程组在这个新进程组中,子进程成为这个进程组的首进程以使该进程脱离所有终端*/ 
    setsid();//因为上边已经结束了父进程使子进程成为后台进程,所以可以直接调用setsid
    ````

* 为禁止 进程重新打开控制终端,经过以上步骤,进程已成为一个无终端的会话组长,但是组长可以重新申请打开一个终端,为了避免这种情况发生,可以通过该进程不再是会话组长来实现.
```
/*在新建的会话中,再次建立一个子进程(孙子),退出该父进程(儿子),保证了该进程(新孙子进程)不是进程组长,同时让该进程无法打开一个新的终端(无组长无法重新申请打开终端)*/
pid = fork();
if(pid > 0)
exit(0);
else if(pid <0)
return -1;
```

* 关闭不再需要的文件描述符.
  创建的子进程从父进程继承打开的文件描述符,如不关闭 会导致系统资源浪费,造成进程所在文件系统无法卸下,和无法预料的错误
```
/*先得到最高文件描述符,然后用一个循环程序,关闭0到最高文件所有的文件描述符*/
for(i=0; i<NOFILE; close(i++));
```

* 将当前目录改成根目录,当守护进程工作在一个装配文件系统时,该文件系统不能被卸载,所以一般需要将当前目录改为根目录.
```
/*将当前目录改为根目录,使进程不与任何文件系统联系*/
chdir("/");
```

* 将文件创建时使用的文件屏蔽字设为0 ,进程从创建它的父进程那里继承的文件屏蔽字可能会拒绝某些许可权,为防止这一点,使用umask(0),将文件屏蔽字清零.
```
/*将文件屏蔽字设为0*/
umask(0);
```
 * 处理SIGCHLD信号,在Linux下,可以简单将SIGCHLD信号操作设成SIG_IGN,这样子进程结束时候不会产生僵尸进程.

 ```
/*忽略SIGCHLD信号*/
singnal(SIGCHLD,SIG_IGN);
```
* syslog();记录系统记录
LOG_INFO:情报信息,正常系统消息


ps -ef查看进程状态
![查看进程状态][2]


  [1]: https://img-blog.csdn.net/20180731080952941?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tra2tkZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70
  [2]: https://img-blog.csdn.net/20180731091628675?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tra2tkZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70
  注:ps -ef查看进程状态
  
      守护进程没有控制终端，因此当某些情况发生时，不管是一般的报告性信息，还是需由管理员处理的紧急信息，都需要以某种方式输出。Syslog 函数就是输出这些信息的标准方法，它把信息发送给 syslogd 守护进程。

  
###总结
Linux大多数服务都是通过守护进程实现的，完成许多系统任务
让某个进程不因为用户、终端或者其他的变化而受到影响，那么就必须把这个进程变成一个守护进程.
编写守护进样实际上是把一个普通进程按照上述的守护进程的特性改造成为守护进程。
