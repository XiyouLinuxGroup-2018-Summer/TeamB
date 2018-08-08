/*************************************************************************
	> File Name: errshow.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月07日 星期二 16时30分31秒
 ************************************************************************/
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
FILE *open_file(char *filename)
{
    FILE *stream;
    errno=0;
    stream=fopen(filename,'r');
    if(stream==NULL)
    {
        printf("can not open file %s.reason:%s\n",filename,strerror(errno));
        exit(-1);
    }
    else 
        return stream;
}
int main(void)
{
    char *filename="test";
    open_file(filename);
    return 0;
}
