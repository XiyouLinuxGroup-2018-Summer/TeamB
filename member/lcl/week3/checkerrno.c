/*************************************************************************
	> File Name: checkerrno.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月07日 星期二 16时27分41秒
 ************************************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
int main(void)
{
    FILE *stream;
    char *filename="test";
    errno=0;
    stream=fopen(filename,"r");
    if(stream==NULL)
        printf("open file %s failed,errno is %d\n",filename,errno);
    else
        printf("open file %s successfully\n",filename);
}
