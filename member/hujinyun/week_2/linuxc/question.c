#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<signal.h>
#include<sys/param.h>
#include<syslog.h>
#include<time.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
	int i;
	pid_t pid;
//	pid=fork();

	for(i=0; i<3; i++)
	{
		pid = fork();
	
		switch(pid)
		{
			case 0: printf("*\n"); break;
			case -1:printf("process error"); break;
			default: printf("*\n"); break;
		
		}
	
	}
	exit(0);

}
