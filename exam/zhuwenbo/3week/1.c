#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
void thread1(void)
{
	printf("A");
}

void thread2(void)
{
	pthread_t tid1;
	int s;
	pthread_create(&tid1, NULL, (void *)thread1, NULL);
	pthread_join(tid1,(void *)&s);
	printf("B");
}

void thread3(void)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex);
		pthread_t tid2;
		int s;
		pthread_create(&tid2, NULL, (void *)thread2, NULL);
		pthread_join(tid2, (void *)&s);
		printf("C");
		pthread_mutex_unlock(&mutex);
	}
}

int main()
{
	pthread_t tid3;
	int s;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_create(&tid3, NULL, (void*)thread3, NULL);
	do{
		pthread_cond_signal(&cond);
	}while(1);
	printf("\n");
	exit(0);
}
