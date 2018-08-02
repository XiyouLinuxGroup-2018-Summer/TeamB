/*************************************************************************
	> File Name: execve.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月02日 星期四 08时56分12秒
 ************************************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main(int argc,char *argv[],char **environ)
{
    pid_t pid;
    int stat_val;
    printf("exec example!\n");
    pid=fork();
    switch(pid)
    {
        case -1:
        {
            perror("process creation failed\n");
            exit(1);
        }
        case 0:
        {
            printf("child process is running\n");
            printf("My pid=%d,parentpid=%d\n",getpid(),getppid());
            printf("uid=%d,gid=%d\n",getuid(),getgid());
            execve("processimage",argv,environ);
            printf("process never go to here!\n");
            exit(0);
        }
        default:
        {
            printf("parent process is running\n");
            break;
        }
    }
    wait(&stat_val);
    exit(0);
}
