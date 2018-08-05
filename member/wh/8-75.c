#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
void *thread1(void)
{
	pthread_t thid = pthread_self();
	printf("thread1 %zu is running\n",thid);
}
void *thread2(void)
{
	pthread_t thid = pthread_self();
	printf("thread2 %zu is running\n",thid);
}
int main(void)
{
	pthread_t thid1,thid2;
	pid_t pid1,pid2,pid3;
	if((pid1 = fork()) < 0 ) {
		perror("");
		exit(1);
	}
	else if(pid1 == 0)	{			//子进程里面
		printf("the  process%d is running\nIt's parent ID is %d\n",getpid(),getppid());
		pthread_create(&thid1,NULL,(void *)thread1,NULL);
		pthread_create(&thid2,NULL,(void *)thread2,NULL);
		if((pid2 = fork()) < 0) {
			perror("");
			exit(1);
		}
		else if(pid2 == 0) {			//子进程里面
			printf("the process%d is running\nIt's parent ID is %d\n",getpid(),getppid());
			pthread_create(&thid1,NULL,(void *)thread1,NULL);
			pthread_create(&thid2,NULL,(void *)thread2,NULL);
		}
		else {
			printf("the process %d is running\nIt's parent ID is %d\n",getpid(),getppid());
			pthread_create(&thid1,NULL,(void *)thread1,NULL);
			pthread_create(&thid2,NULL,(void *)thread2,NULL);
		}
	}
	else {
		printf("the  process%d is running\n It's parent ID is %d\n",getpid(),getppid());
		pthread_create(&thid1,NULL,(void *)thread1,NULL);
		pthread_create(&thid2,NULL,(void *)thread2,NULL);
		if((pid3 = fork()) < 0) {
			perror("");
			exit(1);
		}
		else if(pid2 == 0) {			//子进程里面
			printf("the  process%d is running\nIt's parent ID is %d\n",getpid(),getppid());
			pthread_create(&thid1,NULL,(void *)thread1,NULL);
			pthread_create(&thid2,NULL,(void *)thread2,NULL);
		}
		else {
			printf("the process %d is running\nIt's parent ID is %d\n",getpid(),getppid());
			pthread_create(&thid1,NULL,(void *)thread1,NULL);
			pthread_create(&thid2,NULL,(void *)thread2,NULL);
		}
	}
	return 0;
}
		
		
