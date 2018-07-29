#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int main(void)
{
	pid_t pid;
	char *msg;
	int k;
	printf("Process Creation study\n");
	pid = fork();
	switch(pid) {
		case 0:
			msg = "child process is running";
			k = 3;
			break;
		case -1:
			msg = "process creation failed";
			break;
		default:
			msg = "parent process is running";
			k = 5;
			break;
	}
	while(k > 0)
	{
		puts(msg);
		sleep(3);		
		k--;
	}
	exit(0);
}
