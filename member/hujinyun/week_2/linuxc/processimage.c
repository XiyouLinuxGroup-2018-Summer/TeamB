#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main(int argc ,char **argv,char **environ)
{
	int i;
	printf("this a children process.pid is %d  , parent pid is  %d\n",getpid(),getppid());
	printf("the  uid is %d, gid is %d\n",getuid(),getgid());//进程的实际用户id,和实际用户组id
	
	for(i = 0;i < argc; i++ )
	{
		printf("argv[%d] = %s\n", i, argv[i]);
	
	}


}
