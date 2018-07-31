/*************************************************************************
	> File Name: env.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月31日 星期二 15时28分29秒
 ************************************************************************/

#include<stdio.h>
#include<malloc.h>
extern char **environ;
int main(int argc,char *argv[])
{
    int i;
    printf("Argument:\n");
    for(i=0;i<argc;i++)
        printf("argc[%d] is %s\n",i,argv[1]);
    printf("Environment:\n");
    for(i=0;environ[i]!=NULL;i++)
        printf("%s\n",environ[i]);
    return 0;
}
