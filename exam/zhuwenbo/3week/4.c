#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<ctype.h>
#include<pthread.h>

#define MAX_THREAD 3 //线程个数
unsigned long long main_counter = 0, counter[MAX_THREAD];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *thread_worker(void *arg);

int main()
{
	int i, rtn, ch;
	int tmp[MAX_THREAD];
	pthread_t pthread_id[MAX_THREAD] = {0};
	for(i = 0; i < MAX_THREAD; i++)
	{
		pthread_create(&pthread_id[i], NULL,thread_worker,(void *)&i);
	}
	do
	{
		unsigned long long sum = 0;
		//求所有线程的counter和
		for(i = 0; i < MAX_THREAD; i++)
		{
			sum += counter[i];
			printf("%llu\t", counter[i]);
		}
		printf("%llu\t / %llu", main_counter, sum);
	}while((ch = getchar()) != 'q');
}

void* thread_worker(void *arg)
{
	int thread_num;
	thread_num = *(int *)arg;
	for(;;)
	{
		counter[thread_num]++;
		main_counter++;
	}
}
