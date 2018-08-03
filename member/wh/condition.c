#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
pthread_mutex_t	mutex;	//互斥锁
pthread_cond_t	cond;	//条件变量

void *thread1(void *arg)
{
	pthread_cleanup_push(pthread_mutex_unlock,arg);		//用于释放线程资源,与pthread_cleanup_pop成对出现
	while(1) {
		printf("thread1 is running\n");
		pthread_mutex_lock(&mutex);							//加锁
		pthread_cond_wait(&cond,&mutex);					//释放由参数mutex指向的锁,使当前线程关于条件变量cond指向的条件变量阻塞
		printf("thread1 applied the condition\n");
		pthread_mutex_unlock(&mutex);						//解锁
		sleep(4);
	}
	pthread_cleanup_pop(0);
}

void *thread2(void *arg)
{
	while(1) {
		printf("thread2 is running\n");
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond,&mutex);
		printf("thread2 applied the condition\n");
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

int main(void)
{
	pthread_t tid1,tid2;
	printf("condition variable study!\n");
	pthread_mutex_init(&mutex,NULL);						//初始化一个锁
	pthread_cond_init(&cond,NULL);							//初始化一个条件变量
	pthread_create(&tid1,NULL,thread1,NULL);
	pthread_create(&tid2,NULL,thread2,NULL);
	do {
		pthread_cond_signal(&cond);
	}while(1);
	sleep(50);
	pthread_exit(0);
}
