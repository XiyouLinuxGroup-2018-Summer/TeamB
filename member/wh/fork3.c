#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main(void)
{
	pid_t pid;
	pid = fork();
	int i = 6;
	switch(pid) {
		case 0:
			while(i-- > 3 ) {
				printf("A background process,PID: %d\n,ParentID: %d\n",getpid(),getppid());
				sleep(3);
			}
		case -1:
			perror("Process creation failed!\n");
			exit(-1);
		default:
			printf("I am parent process,my pid is %d\n",getpid());
			return 0;

	}
	return 0;
}
