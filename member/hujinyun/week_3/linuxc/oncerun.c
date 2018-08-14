#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
pthread_once_t once = PTHREAD_ONCE_INIT;

void run(void)
{
    printf("Fuction run is running in thread %u \n",pthread_self());
}
   
void *thread1(void *arg)
{
    pthread_t thid;
    printf("Current thread1's ID=%u",pthread_self());
    pthread_once(&once,run);//once为全局变量,是一个标识,run是一个函数,整个pthread_once函数表示,run函数只能执行一次即第一次执行
                            //int pthreaad_once(pthread_once_t *once_control, void (*init_routine)(void))
    printf("thread1 ends\n");
}

void * thread2(void *arg)
{
    pthread_t thid = pthread_self();
    printf("Current thread2's ID is %u\n",thid);
    pthread_once(&once, run);
    printf("thread2 ends\n");
}
void *thread1(void *arg)
{
    pthread_t thid;
    printf("ID=%u",pthread_self());
    pthread_once(&once,run);//once为全局变量,是一个标识,run是一个函数,整个pthread_once函数表示,run函数只能执行一次即第一次执行
    printf("thread1 ends\n");
}
int main(void)
{
    pthread_t thid1,thid2;

    pthread_create(&thid1,NULL,thread1,NULL);
 //   pthread_create(&thid1, NULL, thread1, NULL);
 //   pthread_create(&thid2, NULL, thread2, NULL);
    sleep(3);
    printf("main thread exit!\n");
    exit(0);
}
