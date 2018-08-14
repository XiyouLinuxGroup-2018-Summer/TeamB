/*************************************************************************
	> File Name: jointhread.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月06日 星期一 12时21分11秒
 ************************************************************************/
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
void assisthread(void *arg)
{
    printf("i am helping to do some jobs\n");
    sleep(3);
    pthread_exit(0);
}
int main(void)
{
    pthread_t assistthid;
    int status;
    pthread_create(&assistthid,NULL,(void *)assisthread,NULL);
    pthread_join(assistthid,(void *)&status);
    printf("assistthid's exit is caused %d\n",status);
    exit(0);
}
