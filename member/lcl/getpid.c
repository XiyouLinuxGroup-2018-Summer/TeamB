/*************************************************************************
	> File Name: getpid.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月02日 星期四 10时45分28秒
 ************************************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main(void)
{
    pid_t pid;
    if((pid=fork())==-1)
    {
        printf("fork error!\n");
        exit(1);
    }
    if(pid==0)
        printf("getpid return %d\n",getpid());
    exit(0);
}
