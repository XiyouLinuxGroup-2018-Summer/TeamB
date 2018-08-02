/*************************************************************************
	> File Name: wait.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月02日 星期四 10时16分00秒
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
int main(void)
{
    pid_t pid;
    char *msg;
    int k;
    int exit_code;
    printf("study how to get exit code!\n");
    pid=fork();
    switch(pid)
    {
        case 0:
        {
            msg="child process is running";
            k=5;
            exit_code=37;
            break;
        }
        case -1:
        {
            perror("process creation failed\n");
            exit(1);
        }
        default:
        {
            exit_code=0;
            break;
        }
    }
    /*父进程都会执行以下这段代码，子进程的pid=0，父进程的pid=子进程的ID*/
    if(pid!=0)//父进程等待子进程结束
    {
        int stat_val;
        pid_t child_pid;
        child_pid=wait(&stat_val);
        printf("child process has exited,pid=%d\n",child_pid);
        if(WIFEXITED(stat_val))
            printf("child exited with code %d\n",WEXITSTATUS(stat_val));
        else
            printf("child exited abnormally\n");
    }
    else//子进程暂停5秒，在这个过程中可以运行命令ps aux查看父进程状态
    {
        while(k-->0)
        {
            puts(msg);
            sleep(1);
        }
    }
    exit(exit_code);
}
