#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
/*
	int k;
	char *string;
	pid_t pid;
	pid = fork();

	switch(pid)
	{
		case 0: printf("I am children,PID:%d,parent,ID is:%d\n",pid,getppid());
			k = 3;
			break;
		case -1: printf("process creation failed\n");
			 break;

		default: printf("I am parent,ID is:%s,now process pid is:%d\n",getpid(),getpid());
			 printf("i am parent.\n");
			
			 k = 5;
			 break;
	
	}
	while(k<5)
	{

	
	}


	*/



	pid_t pid;
	pid = fork();
	switch(pid)
	{
		case 0: while(1)
			{
				 printf("I am children,PID:%d,parent,ID is:%d\n",pid,getppid());
				 sleep(3);
			}
		case -1: printf("process creation failed\n");
			 exit(-1);
		default: printf("I am parent,ID is:%s,now process pid is:%d\n",pid,getpid());
			exit(0);	
	
	}
	return 0;
//	exit(0);
	//return 0;不要用return0,用exit(o);
}
