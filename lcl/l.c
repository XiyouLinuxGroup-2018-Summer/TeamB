/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 15时49分00秒
 ************************************************************************/
//--------L首字母大写
#include<stdio.h>
#include<ctype.h>
#include<string.h>
int main(void)
{
    int i;
    char num[101];
    while(gets(num)!=NULL)
    {
        num[0]=toupper(num[0]);
        for(i=1;i<strlen(num);i++)
        {
            if(num[i]==' ')
                num[i+1]=toupper(num[i+1]);
        }
        printf("%s\n",num);
    }
}







