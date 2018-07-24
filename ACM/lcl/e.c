/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 15时49分00秒
 ************************************************************************/
//--------E计算器
#include<stdio.h>
int main(void)
{
    int n,a,b;
    double sum;
    char op;
    scanf("%d",&n);
    while(n--)
    {
        getchar();
        scanf("%c%d%d",&op,&a,&b);
        if((a>0)&&(b<10000))
        {
            if(op=='+')
                printf("%d\n",a+b);
            else if(op=='-')
                printf("%d\n",a-b);
            else if(op=='*')
                printf("%d\n",a*b);
            else if(op=='/')
            {
                if(a%b==0)
                    printf("%d\n",a/b);
                else
                {
                    sum=a*1.0/b;
                    printf("%.2lf\n",sum);
                }
            }
        }
    }
}





