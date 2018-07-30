/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 15时49分00秒
 ************************************************************************/
//--------F数字字符统计
#include<stdio.h>
#include<string.h>
int main(void)
{
    int n,i,f;
    char num[100];
    scanf("%d",&n);
    getchar();
    while(n--)
    {
        f=0;
        gets(num);
        for(i=0;i<strlen(num);i++)
        {
            if((num[i]=='1')||(num[i]=='2')||(num[i]=='3')||(num[i]=='4')||(num[i]=='5')||(num[i]=='6')||(num[i]=='7')||(num[i]=='8')||(num[i]=='9')||(num[i]=='0'))
                f++;
        }
        printf("%d\n",f);
    }
}



