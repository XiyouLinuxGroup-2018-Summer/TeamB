/*************************************************************************
	> File Name: createthread.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月06日 星期一 10时38分00秒
 ************************************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
//#include<libpthread.a>
int * thread(void *arg)
{
    pthread_t newthid;
    newthid=pthread_self();
    printf("this is a new thread,thread ID is:%lu\n",newthid);
    return NULL;
}
int main(void)
{
    pthread_t thid;
    printf("main thread,ID is %lu\n",pthread_self());//打印主线程的ID
    if(pthread_create(&thid,NULL,(void *)thread,NULL)!=0)
    {
        printf("thread creation failed\n");
        exit(1);
    }
    sleep(1);
    exit(0);
}

