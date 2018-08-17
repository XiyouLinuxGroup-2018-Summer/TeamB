/*************************************************************************
	> File Name: test_select.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月08日 星期三 15时47分11秒
 ************************************************************************/
#include<sys/time.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
void display_time(const char *string)
{
    int seconds;
    seconds=time((time_t*)NULL);
    printf("%s,%d\n",string,seconds);
}
int main(void)
{
    fd_set readfds;
    struct timeval timeout;
    int ret;
    /*监视文件描述符0是否有数据输入，文件描述符0表示标准输入，即键盘输入*/
    FD_ZERO(&readfds);//开始使用一个描述符集合前一般要将其清空
    FD_SET(0,&readfds);
    /*设置阻塞时间为10秒*/
    timeout.tv_sec=10;
    timeout.tv_usec=0;
    while(1)
    {
        display_time("berore select");
        ret=select(1,&readfds,NULL,NULL,&timeout);
        display_time("after select");
        switch(ret)
        {
            case 0:
                {
                    printf("No data in ten seconds\n");
                    exit(0);
                    break;
                }
            case -1:
                {
                    perror("select");
                    exit(1);
                    break;
                }
            default:
                {
                    getchar();//将数据读入，否则标准输入上将一直：为读就绪
                    printf("Data is available now\n");
                }
        }
    }
}