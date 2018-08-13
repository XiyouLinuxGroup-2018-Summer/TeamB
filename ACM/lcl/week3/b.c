/*************************************************************************
	> File Name: b.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月12日 星期日 23时02分47秒
 ************************************************************************/
#include<stdio.h>
int f(int n)
{
    long long int m=0;
    if(n<1)
        return 0;
    if(n==1)
        return 1;
    if(n==2)
        return 2;
    if(n==3)
        return 3;
    if(n==5)
        return 5;
    if(n>3)
    {
        m+=1+n/2+n/3+f(n-5);
        return m;
    }
}
int main(void)
{
    int n;
    int t;
    while(scanf("%d",&n)!=EOF)
    {
        t=f(n);
        printf("%d\n",t);
    }
}

