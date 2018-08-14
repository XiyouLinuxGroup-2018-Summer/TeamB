/*
*多路复用:
*向系统登记希望坚实的套接字,然后阻塞.当套接字上有时间发生时(如有数据到达),
*系统通知服务器进程告知那个套接字发生了什么事件,服务器进程查询对应套接字并进行处理.
*这种工作状态下,若套接字没有事件发生,则服务器进程不会去查询套接字的状态从而不回去浪费CPU的时间,提高了效率
*/
#include<stdio.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
            
void display_time(const char *string)
{
    int seconds;
    //自1970.1.1起的小时数
    seconds = time((time_t *)NULL);
    printf("%s, %d\n",string, seconds);
}

int main(void)
{
    fd_set readfds;
    struct timeval timeout;
    int ret;

    /*监视文件描述符0是否有数据输入,文件描述符0表示标准输入,即键盘输入*/
    FD_ZERO(&readfds); //开时使用一个文件描述符一般都要将其清空
    FD_SET(0,&readfds); //监视标准输入

    /*开始设置阻塞时间为10秒*/
 /*   timeout.tv_sec=10;//秒数
    timeout.tv_usec=0;//微秒数*/

    while(1)
    {
          /*开始设置阻塞时间为10秒*/
        timeout.tv_sec=10;//秒数
        timeout.tv_usec=0;//微秒数 */

        display_time("before select");
        ret = select(1,&readfds,NULL,NULL,&timeout);
        display_time("after select");

        switch(ret)
        {
            case 0: printf("no data in 10 seconds.\n");
                    exit(0);
                    break;
            case -1: perror("select");
                     exit(1);
                     break;
            default: getchar();
                     printf("Data is avaliable now.\n"); 
        }
    }
    return 0;
}
