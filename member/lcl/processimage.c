/*************************************************************************
	> File Name: processimage.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月02日 星期四 08时47分00秒
 ************************************************************************/
/*用来替换进程映像的程序*/
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
int main(int argc,char *argv[],char **environ)
{
    int i;
    printf("i am a process image!\n");
    printf("my pid=%d,parentpid=%d\n",getpid(),getppid());
    printf("uid=%d,gid=%d\n",getuid(),getpid());
    for(i=0;i<argc;i++)
    {
        printf("argv[%d]:%s\n",i,argv[i]);
    }
}

