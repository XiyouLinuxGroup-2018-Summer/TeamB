#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
extern int errno;
int main()
{
	int fd;
	printf("uid study: \n");
	printf("process's uid = %d,enuid = %d\n",getuid(),geteuid());//进程实际用户ID,进程有效用户ID

	//etrerror函数用于指定错误码的提示信息
	if((fd = open("test.c",O_RDWR,S_IRUSR|S_IWUSR)) == -1)
	
	{
		printf("open failure, errno is %d : %s \n",errno,strerror(errno));
		exit(1);
	}
	else
		printf("open success\n");

	close(fd);
	exit(0);




}
