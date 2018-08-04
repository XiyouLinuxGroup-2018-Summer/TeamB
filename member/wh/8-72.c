#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
int num = 0;
void *thread1(void)
{
	num+=1;
}

void *thread2(void)
{
	num+=2;
}

void *thread3(void)
{
	num+=3;
}

int main(void)
{
	
	pthread_t tid1,tid2,tid3;
	pthread_create(&tid1,NULL,(void *)thread1,NULL);
	printf("%d\n",num);
	printf("%d\n",num);
	pthread_create(&tid2,NULL,(void *)thread2,NULL);
	printf("%d\n",num);
	printf("%d\n",num);
	pthread_create(&tid3,NULL,(void *)thread3,NULL);
	printf("%d\n",num);
	printf("%d\n",num);
}
