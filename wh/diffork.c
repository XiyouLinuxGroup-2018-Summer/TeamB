#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int globVar = 5;
int main(void)
{
	pid_t pid;
	int var = 1,i;
	printf("fork is diffrent with vfork\n");
	//pid = fork();
	pid = vfork();
	switch(pid) {
		case 0:
			i = 3;
			while(i-- > 0) {
				printf("Child process is running\n");
				globVar++;
				var++;
				sleep(2);
			}
			printf("Child's glonVar = %d,var = %d\n",globVar,var);
			break;
		case -1:
			perror("Process creation failed\n");
			break;
		default:
			i = 5;
			while(i-- > 0) {
				printf("Parent process is running\n");
				globVar++;
				var++;
				sleep(2);
			}
			printf("parent's globVar = %d,var = %d\n",globVar,var);
			exit(1);
	}
	return 0;
}
