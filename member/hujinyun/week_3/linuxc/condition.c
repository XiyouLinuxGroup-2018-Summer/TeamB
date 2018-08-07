#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

void * thread1(void *arg)
{
    pthread_cleanup_push(pthread_mutex_unlock, &mutex); //中断自动释放资源

    while(1)
    {
        printf("thread1 is running !\n");
        pthread_mutex_lock(&mutex); //加锁
        pthread_cond_wait(&cond, &mutex); //由于条件变量阻塞,无条件等待
        printf("thread1 applied the condition\n");
        pthread_mutex_unlock(&mutex); //解锁
        sleep(4);
    }
    pthread_cleanup_pop(0); //中断自动释放资源
}

void * thread2(void *arg)
{
    while(1)
    {
        printf("thread2 is running !\n");
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
    pthread_mutex_init(&mutex, NULL); //初始化一个互斥锁
    pthread_cond_init(&cond,NULL); //初始化一个条件变量
    pthread_create(&tid1, NULL, (void*)thread1, NULL); //创建一个新线程
    pthread_create(&tid2, NULL, (void*)thread2, NULL); //创建一个新线程

    do{
        pthread_cond_signal(&cond); //解除特定线程的阻塞,存在多个等待线程时按入队顺序激活其中一个
    }while(1);

    sleep(50);
    pthread_exit(0);
}