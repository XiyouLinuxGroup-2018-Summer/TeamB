#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int g = 0;

void thread1(void)
{
	for(int i = 1; i < 50000; i++)
	{
		g += 1;
	}
}


void thread2(void)
{
	for(int i = 1; i < 50000; i++)
	{
		g += 1;
	}
}
int main()
{
	pthread_t tid1, tid2;
	int s;
	pthread_create(&tid1, NULL, (void *)thread1, NULL);
	pthread_join(tid1, (void*)&s);
	pthread_create(&tid2, NULL, (void *)thread2, NULL);
	pthread_join(tid2, (void*)&s);
	printf("%d\n", g);
	pthread_exit((void *)&s);
}
