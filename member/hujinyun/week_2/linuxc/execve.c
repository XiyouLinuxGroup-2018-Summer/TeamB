#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
int main(int argc, char **argv, char **environ)
{
	pid_t pid;
	int stat_val;
	printf("enec example!\n"); 
	pid = fork();
	switch(pid)
	{
		case 0: printf("I am child process,id is %d, my parent process id is %d ",getpid(),getppid());
			printf("uid is %d ,gid is%s",getuid(),getgid());
			execve("processimage",argv,environ);
			printf("never acheive ahhahahahah\n");
			break;//exit(0);
		case -1:printf("error process.");
			exit(0);
		default:
			printf(" parent process is running\n");
	//		wait(&stat_val);
			break;
	
	}

	wait(&stat_val);
	exit(0);

	

}
