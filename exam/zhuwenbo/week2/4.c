#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
int main(int argc, char *argv[])
{
	printf("Hello, my pid is %d\n", getpid());      //获取进程ID
	pid_t pid = fork();                             //fork创建子进程
	if (pid == 0)                                   //如果fork参数返回0，执行子进程
	{
		printf("I was forked! :D %d\n", getpid());
	}
	else                                         
	{
		waitpid(pid, NULL, 0);          //waitpid函数，用来将父进程挂起，等进程号为pid的子进程结束后在执行
		printf("%d was forked!\n", pid);
	}
	return 0;
}
/*代码输出是
Hello, my pid is ....(父进程ID)
I was forked :D ....(子进程ID)
... was forked!     (子进程ID)
*/

/* test | cat
输出结果为
Hello, my pid is ....
I was forked :D....
Hello, my pid is ....
... was forked!
原因应该是遇到管道操作，会自动创建一个子进程，让子进程执行a.out然后等a.out结束后执行,再执行一次进程,从main开始执行，此时子进程已经结束，所以waitpid就不会挂起。所以会产生这样的结果。



*/

