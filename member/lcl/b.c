/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 15时49分00秒
 ************************************************************************/
//--------B球体积
#include<stdio.h>
int main(void)
{
    char num[3];
    char a;
    int i=0,j=0;
    while(scanf("%s",num)!=EOF)
    {
        for(j=0;j<3;j++)
        {
            for(i=0;i<2;i++)
            {
                if(num[i]>num[i+1])
                {
                    a=num[i];
                    num[i]=num[i+1];
                    num[i+1]=a;
                }
            }
        }
        for(i=0;i<2;i++)
            printf("%c ",num[i]);
        printf("%c\n",num[2]);
    }
}


