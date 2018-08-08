#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
void *assisthread(void*arg)
{
    printf("I am helping to do some jobs.\n ");
    sleep(5);
    pthread_exit(0); //退出码
}
int main(void)
{
    pthread_t assistthid;
    int status;

    pthread_create (&assistthid, NULL, assisthread,NULL);
    pthread_join(assistthid,(void *)&status); //阻塞主线程,待线程assisthread结束. //注意返回值类型相对应
    printf("assistthread's exit is cased %d\n", status);

    return 0;
}