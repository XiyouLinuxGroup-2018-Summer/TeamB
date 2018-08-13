# 线程控制之条件变量

标签： 线程的可结合和分离

---
上一次我写了互斥锁,这次来写一下总能用到互斥锁的条件变量.
因为要保证条件变量能被正常的修改,条件变量就要受到特殊保护,实际使用的过程中互斥锁扮演者这样一个必不可少的保护者的角色.

>条件变量是利用线程间共享的全局变量进行同步的一种机制.(条件变量宏观上类似于if语句,符合条件就能执行某段程序,否则只能等待条件成立 )
使用条件变量主要包括两个动作:①一个等待使用资源的线程,等待"条件变量被设置为真"; ②另一个线程在使用完资源后"设置条件为真".
这样就可以实现线程间的同步了.

同样来举个栗子: 别人给你提供了了办公室,还提供了锁子,但你被制造出来后`pthread_cond_init`,就像你玩手机(老式手机),你本该开开心心工作,但是你的主人要求你只要遇到等待`pthread_cond_wait(无条件等待)` `pthread_cond_timewait(计时等待)`你就必须停在那里打开你的锁,让出房间给别人,直到你接收到了继续工作的信号`pthread_cond_signal(解除特定等待,只激活一个)` `pthread_cond_broadcast(解除所有的线程阻塞)`,那么你还需要跟后来还在那个房间工作的人竞争一下房间的使用权,看看他还剩分配的多少时间或者你心情不好直接掐架.那么如果你接收到工作信号,而那个房间刚好没有人,那么你便可以安安心心工作直到有人也接到工作信号前来与你掐架或又有信号让你等待.记得在你进入或者离开的时候带上你的锁子上锁或解锁,以保证安全性.
最后要清除这个条件变量哦`pthread_cond_destory`

操作条件变量的函数使用如下:
初始化条件变量:
```
pthread_cond_t cond;
pthread_cond_init(&cond);
/*或直接*/
pthread_t cond = PTHREAD_COND_INITALIZER;
```
基于条件变量阻塞，无条件等待
```
pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
/*释放由mutex指向的互斥锁，同时使当前线程关于cond指向的条件变量阻塞，直到条件被信号唤醒．*/
```
阻塞直到指定时间发生，计时等待
```
pthread_cond_timewait(pthread_cond_t *cond, pthread_mutex_t *mutex,const struct timespec *abstime);
/*阻塞直到条件变量获得信号或经由abstime指定的时间，如果在给定时刻条件变量没有满足则返回ＥＴＩＭＥＯＵＴ*/
```
解除特定线程的阻塞，存在多个等待线程时按入队顺序激活其中一个
```
pthread_cond_signal(pthread_cond_t *cond);
/*解除特定线程的阻塞，存在多个等待线程时按入队顺序激活其中一个,注意不是激活了就一定立刻执行，还要考虑到别的正在占用同块存储空间的线程*/
```
解除所有线程的阻塞
```
pthread_cond_broadcast(pthrad_cond_t *cond);
/*竞争激烈*/
```
下面是利用互斥锁与条件变量的一个具体实例：
```
/*生产者消费者模型*/
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
}
```
解释下回补充．
这章的坑有不少，踩到了或者别人遇见了，再来补充记录．防止以后再次遇见．


###再来提一下资源释放：
不论是可预见的线程终止还是异常终止,都会存在资源释放的问题.在不考虑运行出错的前提下,如何保证线程终止时能顺利的释放掉自己所占用的资源,特别是锁资源,就是一个必须考虑解决的问题.

最经常出现的情形是资源独占锁的使用:线程为了访问临界资源而加上锁,但在访问过程中被外界取消,如果线程处于响应取消的状态,且采用异步方式响应,或者在打开独占锁以前的运行路径上存在取消点,则该临界资源将永远处于锁定状态而得不到释放.外界取消操作是不可预见的
因此的确需要一个机制来简化用于资源释放的编程.

在POSIX线程API提供了一个pthread_cleanup_push()/pthread_cleanup_pop()函数 用于自动释放资源.
**---从pthread_cleanup_push()的调用点 到 pthread_cleanup_pop()之间的程序段中的终止动作(<mark>包括调用pthread_exit()和取消点终止</mark>)都将执行pthread_cleanup_push()所指定的清理函数.**(只要是在期间有终止操作,都会调用清理函数以防死锁)
<mark>pthread_cleanup_push()带有一个"{",而pthread_cleanup_pop()带有一个"}",因此这两个函数必须成对出现,且必须位于程序的同一级别代码段才能通过编译.</mark>

**以下表示:work终止时,将主动调用pthread_mutex_unlock(mut),以完成解锁工作**
```
pthread_cleanup_push(pthread_mutex_unlock,(void *)&mut);
pthread_mutex_lock(&mut);
/*do some work*/
pthread_mutex_unlock(&mut);
pthread_cleanup_pop(0);
/*本来do some work 之后是有pthread_mutex_unlock(&mut);这句,也就是有解锁操作,但是在do some work 时会出现非正常终止,这样系统会根据pthread_cleanup_push中提供的函数和参数,可进行解锁或者其他状态,以免造成死锁.*/
```
注:
1.如果在线程主函数中调用return,如果return语句包含在pthread_cleanup_push()/pthread_cleanup_pop中,则不会引起清理函数的执行,反而会导致 segment fault这点要注意.
2.清理函数时,最好都应暂时设置成PTHREAD_CANCEL_DEFERRED模式,以便线程处于PTHREAD_CANCEL_ASYNCHRONOUS状态代码会出错.









