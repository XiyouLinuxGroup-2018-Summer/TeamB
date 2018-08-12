#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void thread1(void)
{
	printf("A");
}
void thread2(void)
{
	printf("B");
}
void thread3(void)
{
	printf("C");
}

int main()
{
	pthread_t tid1, tid2, tid3;
	int status;
	while(1)
	{
		pthread_create(&tid1, NULL, (void *)thread1, NULL);
		printf("%ld\n", tid1);
		pthread_join(tid1, (void *)&status);
		pthread_create(&tid2, NULL, (void *)thread2, NULL);
		pthread_join(tid2, (void *)&status);
		pthread_create(&tid3, NULL, (void *)thread3, NULL);
		pthread_join(tid3, (void *)&status);
	}
	pthread_exit((void *)&status);
}
