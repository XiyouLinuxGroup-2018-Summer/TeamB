#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main(int argc,char *argv[],char **envp)
{
	int i;
	printf("I am a precess image!\n");
	printf("my pid = %d,parentid = %d\n",getpid(),getppid());
	printf("uid = %d,gid = %d",getuid(),getgid());

	for(i = 0;i < argc;i++)
		printf("argv[%d]:%s\n",i,argv[i]);
}

