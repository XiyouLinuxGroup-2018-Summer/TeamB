#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main(void)
{
	printf("hello my pid is %d\n",getpid());
	pid_t pid = fork();
	if(pid == 0) {
		printf("I was forked! :%d\n", getpid());	//先执行子进程，等待子进程执行完毕执行父进程
	}
	else{
		printf("1\n");
		waitpid(pid, NULL, 0);
		sleep(2);
		printf("%d was forked!\n", pid);
		sleep(2);
	}
	return 0;
}

//./a.out | cat fork一个子进程去执行a.out,a.out先执行子进程，将输出重定向到文件中，然后关闭写指针，父进程执行但没有写入文件，子进程执行完毕。打开写指针，再次返回，执行父进程将输出写入文件，最后输出文件内容，删除文件
