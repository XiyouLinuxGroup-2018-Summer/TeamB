/*************************************************************************
	> File Name: studyuid.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月02日 星期四 11时10分47秒
 ************************************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
extern int errno;
int main(void)
{
    int fd;
    printf("uid study:\n");
    printf("process's uid=%d,euid=%d\n",getuid(),geteuid());
    //strerror获取指定错误码的提示信息
    if((fd=open("test.c",O_RDWR))==-1)
    {
        printf("open failure,errno is %d:%s\n",errno,strerror(errno));
        exit(1);
    }
    else
        printf("open successfully!\n");
    close(fd);
    exit(0);
}
