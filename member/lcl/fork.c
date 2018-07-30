/*************************************************************************
	> File Name: fork.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月30日 星期一 19时59分56秒
 ************************************************************************/
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
int main(void)
{
    pid_t pid;
    printf("process creation study\n");
    pid=fork();
    switch(pid)
    {
        case 0:
        {
            printf("child process is running,curpid is :%d,parentpid is :%d\n",pid,getppid());
            break;
        }
        case -1:
        {
            perror("process creation failed\n");
            break;
        }
        default:
        {
            printf("parent process is running,children is %d,parentpid is %d\n",pid,getpid());
            break;
        }
    }
}
