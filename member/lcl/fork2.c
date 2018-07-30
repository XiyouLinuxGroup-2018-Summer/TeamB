/*************************************************************************
	> File Name: fork2.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月30日 星期一 20时08分21秒
 ************************************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main(void)
{
    pid_t pid;
    char *mag;
    int k;
    printf("process creation study\n");
    pid=fork();
    switch(pid)
    {
        case 0:
        {
            mag="child process is running";
            k=3;
            break;
        }
        case -1:
        {
            perror("process creation failed\n");
            break;
        }
        default:
        {
            mag="parent process is running";
            k=5;
            break;
        }
    }
    while(k>0)
    {
        puts(mag);
        sleep(1);
        k--;
    }
    exit(0);
}
