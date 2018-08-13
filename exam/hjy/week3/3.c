/*编写一个至少具有三个线程的程序（ 称之为线程 A、 B 和 C）， 其中线程 A
输出字符'A'， 线程 B 输出字符'B'， 线程 C 输出字符'C'。 使得最终输出
结果为“ ABCABCABC……”。*/
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

#define NUM 10

pthread_mutex_t mutex; //指向互斥锁
pthread_cond_t cond;
//conda,condb,condc; //指向条件变量

int flagc = 0;
int flagb = 0;
int flaga = 0;

void *thread1(void *arg)
{
  pthread_cleanup_push(pthread_mutex_unlock, &mutex);

  pthread_mutex_lock(&mutex);
 // pthread_cond_wait(&cond, &mutex);
  printf("A");
  pthread_mutex_unlock(&mutex);

  pthread_cleanup_pop(0);
}

void *thread2(void *arg)
{
  pthread_cleanup_push(pthread_mutex_unlock, &mutex);
    
    pthread_mutex_lock(&mutex);
 //   pthread_cond_wait(&cond, &mutex);
    printf("B");
    pthread_mutex_unlock(&mutex);
  pthread_cleanup_pop(0);
    
}

void *thread3(void *arg)
{
  pthread_cleanup_push(pthread_mutex_unlock, &mutex);
    
    pthread_mutex_lock(&mutex);
  //  pthread_cond_wait(&cond,&mutex);
    printf("C");
    pthread_mutex_unlock(&mutex);
  pthread_cleanup_pop(0);
    
}
int main(void)
{
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;

    pthread_mutex_init(&mutex,NULL); //初始化一个互斥锁
//    pthread_cond_init(&conda,NULL); //初始化一个条件变量
    pthread_cond_init(&cond,NULL);
//    pthread_cond_init(&condc,NULL);
    
while(1)
{
    pthread_create(&tid1, NULL, (void*)thread1, NULL);
    pthread_create(&tid2, NULL, (void*)thread2, NULL);
    pthread_create(&tid3, NULL, (void*)thread3, NULL);

   // sleep(3);
  //  pthread_cond_signal(&cond); //给A发一个信号
    pthread_join(tid1,NULL);
  //  pthread_cond_signal(&cond);
    pthread_join(tid2,NULL);
  //  pthread_cond_signal(&cond);
    pthread_join(tid3,NULL);

}
   
   
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
