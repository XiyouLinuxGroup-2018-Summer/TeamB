/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 15时49分00秒
 ************************************************************************/
//--------K素数
#include<stdio.h>
int f(int sum)
{
    int i=2;
    while(i<sum/2)
    {
        if(sum%i==0)
        {
            return 0;
            break;
        }
        else
            i++;
    }
    if(i==sum/2)
        return 1;
}
int main(void)
{
    int a,b,i,sum,t;
    while(1)
    {
        t=0;
        scanf("%d%d",&a,&b);
        if((a==0)&&(b==0))
            break;
        if((a>=-39)&&(a<b)&&(b<=50))
        {
            for(i=a;i<=b;i++)
            {
                sum=i*i+i+41;
                if(f(sum)==0)
                {
                    t=1;
                    printf("Sorry\n");
                    break;
                }
            }
        }
        if(t!=1)
            printf("OK\n");
    }
}
