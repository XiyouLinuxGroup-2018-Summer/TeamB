#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>
int mian()
{
	pid_t pid;
	char *string;
	int k;
	int exit_code;

	printf("How to get exit code.\n");
	pid = fork();
	switch(pid)
	{
		case -1: perror("process creation failed\n");
			 exit(-1);
		case 0:
			 string = "chile process is running!";
			 k = 5;
			 exit_code = 37;
			 break;
		default: exit_code = 0;
			 break;
	
	}

	/*父子进程都会执行下段代码,子进程pid为0,父进程pid值为子进程ID*/
	if( pid != 0) //父进程等待子进程结束
	{
		int stat_val;
		pid_t child_pid;
		
		child_pid = wait(&stat_val);

		printf("child process has exited,pid = %d\n",child_pid);
		if(WIFEXITED(stat_val))
		{
			printf("child exited with code %d\n",WEXITSTATUS);
		

		}
		else
			printf("chihld exited abnormally\n");

	}
	else
	{
		while(k--)
		{
			puts(string);
			sleep(1);
		}
	
	}
	exit(exit_code);


}
