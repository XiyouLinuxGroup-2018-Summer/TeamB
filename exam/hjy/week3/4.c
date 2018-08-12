/*创建两个线程（ 称之为线程 A 和线程 B） 和一个文件， 文件的最大存储数
据量为 100 字节， 初始数据为空。 线程 A 负责向文件中写入数据（ 写入的
数据自定）， 线程 B 负责从文件中读取数据， 要求： ① A 不可向线程已满
的文件中写入数据或写入数据使之超过 100 字节；②线程 B 不可从空文件
中读取数据且读取量不可大于数据量； ③读取过的数据应从文件中删除；
④程序执行时不出现任何错误*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITALIZER;
pthread_cond_t notfull = PTHREAD_COND_INITALIZER;
pthread_cond_t notempty = PTHREAD_COND_INITALIZER;

int TT = 0;
int flag =1;
(void*)threadA((void*)arg) //write +
{
    while(flag)
    {
        pthread_mutex_lock(&mutex);
        while(TT = 100)
        {
            pthread_cond_wait(&notfull, &mutex); //已经满了,所以需要等待 notfull等待,将mutex指定的锁解锁
        }
        //TT++;
        char *s;
        scanf("%s",s);
        FILE*fp;        
        fp=fopen("example5","rt+");// 文件名  
        fprintf(example5,"%s",s);
        fseek(fp,0L,SEEK_END); /* 定位到文件末尾 */
　　     TT=ftell(fp); /* 得到文件大小 */
        pthread_cond_signal(&notempty);//发出队非空的消息
        pthread_mutex_unlock(&mutex);

    }
    pthread_exit(NULL);

}
(void*)threadB((void*)arg) //clean -
{
    while(flag)
    {
        pthread_mutex_lock(&mutex);
        while(TT=0)
        {
            pthread_cond_wait(&notempty,&mutex);
        }
            FILE*fp;
            fp=fopen("example5","rt+");// 文件名
            fseek(fp,0L,SEEK_SET); /* 定位到文件末尾 */
            pthread_cond_signal(&notfull);
            pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
int main(void)
{
    ptthread_t tid1,tid2;
    int fd;

    if(fd = open("example5",O_CREAT| O_EXCL, S_IRUSR| S_IWUSR ) == -1) //文件的最大存储数据为100字节
    {
        perror("open");
        exit(1);
    }
    pthread_create(&tid1,NULL,(void *)threadA, NULL); //线程A负责向文件中写入数据
    pthread_create(%tid2,NULL,(void *)threadB, NULL); //线程B负责从文件中读取数据
    sleep(1);
    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    return 0;
}
/*生产者消费者模型
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notfull = PTHREAD_COND_INITIALIZER;  //不满
pthread_cond_t notempty = PTHREAD_COND_INITIALIZER; //不空

int TT = 0;
int flag = 1;

void *produce(void *arg) // return  1
{
	while(flag)
	{
		pthread_mutex_lock(&mutex);
		while (TT == 100) // 100
		{
			printf("full! producer is waiting\n");
			pthread_cond_wait(&notfull, &mutex); //等待不满
		}
		TT++;
		printf("我是生产者，我让TT+1,TT现在是%d\n", TT);
		pthread_cond_signal(&notempty); //发出队非空的消息
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
	return (void *)1;
}
void *consume(void *arg) // return  2
{
	while(flag)
	{
		pthread_mutex_lock(&mutex);
		while (TT == 0)
		{
			printf("empty! consumer is waiting\n");
			pthread_cond_wait(&notempty, &mutex); //等待队不空
		}
		TT--;
		printf("我是消费者,我让TT-1,TT现在是%d\n", TT);

		pthread_cond_signal(&notfull); //发出队不满的消息
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
	return (void *)2;
}
int main(void)
{
	pthread_t th1 = 0, th2 = 0;
	int ret1 = 0, ret2 = 0;

	int p1 = pthread_create(&th1, NULL, produce, NULL);
	int p2 = pthread_create(&th2, NULL, consume, NULL);

	printf("th1 == %d ,p1 == %d \n", th1, p1);
	printf("         th2 == %d ,p2 == %d \n", th2, p2);

	sleep(1);
	flag = 0 ;

	int tmp1 = pthread_join(th1, NULL);
	int tmp2 = pthread_join(th2, NULL);

	return 0;
}*/