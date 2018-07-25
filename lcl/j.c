/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 15时49分00秒
 ************************************************************************/
//--------J亲和数
#include<stdio.h>
int f(int m)
{
    int t=1,i=2;
    while(i<=m/2)
    {
        if(m%i==0)
            t+=i+m/i;
        i++;
    }
    return t;
}
int main(void)
{
    int n;
    int a,b,a1,b1;
    scanf("%d",&n);
    while(n--)
    {
        scanf("%d%d",&a,&b);
        if((a>=0)&&(a<=6000000)&&(b>=0)&&(b<=6000000))
        {
            a1=(f(a)+1)/2;
            b1=(f(b)+1)/2;
            if((a1==b)&&(b1==a))
                printf("YES\n");
            else
            printf("NO\n");
        }
    }
}






