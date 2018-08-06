#include<stdio.h>
#include<unistd.h>
int main(void)
{
	for(int i = 0;i < 2;i++) {			//第一次执行完毕成为两个进程，执行printf输出两次helloworld
										//父进程第二次循环又生成子进程，子进程第二次循环也生成子进程，一共四个进程又输出hello world
										//故输出六次helloworld，共有四个进程
		pid_t pid = fork();
		printf("hello world\n");
	}
	return 0;
}
