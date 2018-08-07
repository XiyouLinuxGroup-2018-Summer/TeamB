#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int *thread(void *arg)
{
	pthread_t newthid;
	newthid = pthread_self();                //创建线程
	printf("this is a new thread, thread ID = %lu\n", newthid);
	return NULL;
}

int main(void)
{
	pthread_t thid;
	printf("main thread, ID is %lu\n", pthread_self());   //打印主线程ID
	if( pthread_create(&thid, NULL, (void *)thread, NULL) != 0)
	{
		printf("thread creation failed\n");
		exit(1);
	}
	sleep(1);
	exit(1);
}
