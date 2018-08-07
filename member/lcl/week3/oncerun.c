/*************************************************************************
	> File Name: oncerun.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月06日 星期一 11时12分19秒
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
pthread_once_t once=PTHREAD_ONCE_INIT;
void run(void)
{
    printf("Function run is running in thread %lu\n",pthread_self());
}
void * thread1(void *arg)
{
    pthread_t thid=pthread_self();
    printf("Current thread's ID is %lu\n",thid);
    pthread_once(&once,run);
    printf("thread1 ends\n");
}
void * thread2(void *arg)
{
    pthread_t thid=pthread_self();
    printf("Current thread's ID is %lu\n",thid);
    pthread_once(&once,run);
    printf("thread2 ends\n");
}
int main(void)
{
    pthread_t thid1,thid2;
    pthread_create(&thid1,NULL,thread1,NULL);
    pthread_create(&thid2,NULL,thread2,NULL);
    sleep(3);
    printf("main thread exit!\n");
    exit(0);
}
