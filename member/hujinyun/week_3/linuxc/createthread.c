#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
int *thread(void*arg)
{
    pthread_t  newthid;

    newthid = pthread_self();
    printf("This is a new phread, phread ID = %u\n",newthid); //打印新线程ID
    printf("This is a new phread, phread ID = %d\n",newthid);

    printf("This is a new phread, phread ID = %d\n",pthread_self());
    int a=-5;
    printf("%u\n",a);
    printf("%d\n",a);
    return NULL;
    
}
int main(void)
{
    pthread_t thid;
    printf("This is main phread, phread ID = %d\n",pthread_self());
    if( pthread_create(&thid, NULL, (void*)thread, NULL )!=0 )
    {
        printf("thread creation faailed\n");
        exit(1);
    }
    sleep(1);
    exit(0);
}
