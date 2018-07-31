#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
int main(int argc,char *argv[],char **envp)
{
	pid_t pid;
	int stat_val;
	printf("Exec example!\n");
	pid = fork();
	int i;
	for(i = 0;i < argc;i++)
		printf("argv[%d] is %s\n",i,argv[i]);
	
	printf("argc[2] =  %s\n",argv[2]);
	switch(pid) {
		case -1:
			perror("");
			exit(1);
		case 0:
			printf("Child process is running\n");
			printf("My pid is %d,parentpid = %d\n",getpid(),getppid());
			printf("uid is %d,gid is %d\n",getuid(),getgid());
			execve(argv[1],argv,envp);
			printf("process never go to here!\n");
			exit(0);
		default:
			printf("Parent process is running\n");
			break;
	}
	wait(&stat_val);
	exit(0);
}
