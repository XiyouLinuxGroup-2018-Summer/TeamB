



---

**线程最大的特点就是资源的共享性**,而资源共享中同步问题是多线程编程的难点.
Linux系统提供了多种方式处理**线程间的同步关系**,其中最常用的就是**互斥锁**,**条件变量**和**异步信号**.下面就来让我们看看这些都是怎么用的吧(ˇˍˇ)

##(一)互斥锁


举个栗子:
假如现在有一个办公室每次只能进一人,你现在要在这个房间奋斗早早完成工作好把工作室让给别人用-.-   
但是由于工作需求,你在进行工作的时候会不停进出这个房间,那么在你出去的时候,这就给啦别人可乘之机,        他就会利用这个空挡在现在属于你的办了公室乱捣乱,把你的办公室弄的乱七八糟,或者做一些他的事情.让你回去时摸不着头脑目瞪口呆ing...
那么你该怎么办呢? 就需要给办公室上个锁啦! 
那么你需要做的事是什么呢 (就跟把大象放进冰箱需要几步一个意思(ーー゛))
首先买个锁(`pthread_mutex_init`),然后看看有没有人用你想用的办公室.
有人用(资源被别线程占用,或已经挂了一把锁子)你现在可能会有两个想法涌入脑海: 1.我等!先排队等待别人用完直到轮到你,拿上你的宝贝锁子咔~赶紧锁住要不就被别人抢先啦(`pthread_mutex_lock`). 2.想要的办公室已经有人锁了呢,傲娇脸哼,我不等了!我走!(`pthread_mutex_trylock`)立即返回EBUSY.
没人用或排队排到了:努力工作ing...
工作结束,要走啦解锁(`pthread_mutex_unlock`),把办公室上锁的权利让给其他人,一定要记得开锁哦,你的锁子只有你有钥匙呢.
这是一个机密锁,用完后,记得一定用后即焚!释放房间资源!(`pthread_mutex_destory`)

这是一个从网上看来的更形象的描述和实例.:
**当一个全局的共有资源被多个线程同时调用会出现意想不到的问题**，比如你去银行取出所有钱，同时又转所有钱到支付宝，如果这两块同时执行，就有可能转出双倍的钱，这是不允许的。
这时候要使用的这个线程需要将这个资源（取钱这个过程）先“锁”起来，然后用好之后再解锁，这期间别的线程就无法使用了，其他线程的也是类似的过程。

好了开始正题
互斥锁函数:
```
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t * attr)

int pthread_mutex_lock(pthread_mutex_t *mutex)

int pthread_mutex_unlock(pthread_mutex_t *mutex)

int pthread_mutex_trylock(pthread_mutex_t *mutex)

int pthread_mutex_destroy(pthread_mutex_t *mutex);

```
写一个应用实例
```
#include <pthread.h>
#include <stdio.h>
 
pthread_mutex_t mutex ; //执行互斥锁变量
void *print_msg(void *arg)
{
        int i=0;
        pthread_mutex_lock(&mutex); //上锁
        for(i=0;i<15;i++)
        {
                printf("output : %d\n",i);
                usleep(100);
        }
        pthread_mutex_unlock(&mutex); //解锁
}
int main(int argc,char** argv){
        pthread_t id1; //创建返回线程ID
        pthread_t id2; 
        pthread_mutex_init(&mutex,NULL); //初始化锁
        pthread_create(&id1,NULL,print_msg,NULL);//创建线程1执行printf_msg函数
        pthread_create(&id2,NULL,print_msg,NULL);//同上
         /*创建线程后子线程不一定立即执行,还可能要看时间分配及竞争力*/
        
        pthread_join(id1,NULL); //主线程挂起等待id1所指向的线程终止后释放资源
        pthread_join(id2,NULL); //同上主线程等待id2线程
        //sleep(3);
        pthread_mutex_destroy(&mutex); //清除锁
        return 1;
}

```
<mark>下面开始根据此示例进行猜想测试:</mark>

###1.直接执行上述代码.这种情况就是最理想的情况,将会一个线程一个线程地执行,且互相不会产生影响.![一个一个线程依次执行][1]
  
###2.若此时我们把锁去掉会怎么样呢
  他们两个子线程就会开始竞争了,那情况可能就会说不准啦,有可能是势均力敌,你输出一个,我输出一个,也可能是我比你强,我输出了三个你才抢到时间输出一个,那就是不可预知的了.
  两个线程相当于是同步进行占用者一块资源但是真正一次让一个用看哪个线程的竞争力大喽.
  所以你在以后编程时当然是井井有条,一个线程一个线程执行,那么就一定要记得上锁.
  
  我的电脑拿掉锁跑出来是这样:
 ![无锁][2]


  这里贴一个学长ppt的来更好理解线程运行,顺便看下补充知识点[[一]](#jump)
  ![线程运行][3]
###3.那么假如我拿掉pthread_join(thid),来换成sleep(3)或usleep(1)会怎么样呢?
  和上边输出结果相同,为什么呢
  原来是因为sleep和pthread_join在此处的用法大致相同,pthread_join是让此处的线程(此程序中为主线程)等待直到参数线程结束.
  而sleep用法是让此处的线程(此程序为主线程)等待括号中的秒数,让出cpu让其他线程执行.而3秒已经足够两个线程执行完了.
 
  那想使劲往短时间测试代码可以使用usleep(单位是微秒),假如我将时间设的足够小,只有1微秒,那么就是主线程只睡了1微秒.因为我电脑两个子线程势均力敌,那么跑出来是这样的.
  ![此处输入图片的描述][4]
  两个线程刚各自进行了一次,主线程就醒了,那么主线程开始下执行一条语句抢到了CPU,若主线程结束,子线程们就会一起结束.
  


  [1]: https://img-blog.csdn.net/20180810101442147?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tra2tkZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70
  [2]: https://img-blog.csdn.net/20180810102456928?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tra2tkZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70
  [3]:https://img-blog.csdn.net/20180810103106681?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tra2tkZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70
  [4]: https://img-blog.csdn.net/20180810104608927?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tra2tkZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70
  
### 4.可想而知,若是我加上锁,而让主线程时间睡的很短.
那么就会只进行一个子线程(可能还未执行完或者执行完了根据情况而定),另一个子线程还没有开始执行就结束了.
  所以想让都执行完,要不就让主线程睡的时间久一点,要不就更加精确一点让主线程等待他们执行完毕后再叫醒它.(｡･∀･)ﾉﾞ
  
  还有就是创建了线程让它先睡,有特定的时候让再叫醒子线程"歪,该醒啦该干活了!"子线程就工作啦的情况,就是条件变量,我开一篇新博客写.(｡･∀･)ﾉﾞ
  
  
  <br>
  <br>
  嗯.本来这篇博客可以圆满结束.上边讲到了pthread_mutex_join就补充个知识点吧~
  补充线程知识点:
<span id="jump">[一]</span> :对于单CPU单核,每个线程只有在系统分配给他的时间片内才能取得CPU的控制权,执行线程中的代码

子进程是通过拷贝父进程的地址空间来实现的,而线程与进程内的线程共享程序代码,一段代码可以同时被多个线程执行.
<span id ="jump">[二]</span> :线程的可结合和分离

在任何一个时间点上,线程是可结合的(joinable)或者是分离的(detached).
一个<font color=#DEB887>  可结合的线程 </font> **能够被其他线程收回其资源和杀死**.在被其他线程回收之前,他的存储器资源(例如栈)是不释放的.
相反,一个<font color=#DEB887>  分离的线程 </font>是**不能被其他资源回收或者杀死**的.它的存储资源在它终止时由系统自动释放.

默认情况下,线程被创建成可结合的,而为了避免存储器泄露,每个可结合线程都应该要么被显示地回收,即调用 pthread_join ;要么通过调用 pthread_detach 函数被分离.

如果一个可结合线程结束运行但没有被join,他的状态类似僵尸进程,即还有一部分资源没有被回收,所以创建线程者应该调用pthread_join来等待线程运行结束,并可得到线程的退出代码,回收其资源.


由于**调用pthread_join后,如果该线程没有运行结束,调用者会被阻塞**,有些情况下我们并不希望如此.

例如在web服务器中当主线程为每个新来的连接请求创建一个子线程进行处理的时候,主线程**并不希望**调用pthread_join而 **阻塞**(因为**还要继续处理之后到来的连接请求**),这是可以在子线程中加入代码`pthread_detach(pthread_self())`;**或者**父线程中调用`pthread_detach(thread_id)`(非阻塞,可立即返回).
这将该子进程状态设为分离的(detached),**如此一来,该线程运行结束后会自动释放所有资源.(自己释放)**

一个可"join"的线程所占用的内存,仅当有线程对其执行了pyhread_join()后才会释放(其他线程也可以释放它).
**因此为了避免内存泄漏,所有的进程终止时,要么已被设为DETACHED,要么使用pthread_join()来回收资源.**
注:一个线程不能被多个线程等待

OK,结束,想到了再补充