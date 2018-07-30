/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 15时49分00秒
 ************************************************************************/
//--------D最小公倍数
#include<stdio.h>
int f(int a,int b)
{
    int i=2,sum=1;
    while(i<=b)
    {
        if((a%i==0)&&(b%i==0))
        {
            sum*=i;
            a/=i;
            b/=i;
        }
        else
            i++;
    }
    sum=sum*a*b;
    return sum;
}
int main(void)
{
    int a,b;
    while(scanf("%d%d",&a,&b)!=EOF)
    {
        printf("%d\n",f(a,b));
    }
}


