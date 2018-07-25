/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 15时49分00秒
 ************************************************************************/
//--------G数字序列插入
#include<stdio.h>
int main(void)
{
    int n,m,j,f=0,i;
    while(1)
    {
        f=0;
        scanf("%d%d",&n,&m);
        if(n==0&&m==0)
            break;
        else
        {
            int num[n+1];
            for(i=0;i<n;i++)
                scanf("%d",&num[i]);
            for(i=0;i<n;i++)
            {
                if(m<num[i])
                {
                    f=1;
                    j=n;
                    while(j>i)
                    {
                        num[j]=num[j-1];
                        j--;
                    }
                    num[i]=m;
                    break;
                }
            }
            if(f!=1)
                num[n]=m;
            for(i=0;i<n;i++)
                printf("%d ",num[i]);
            printf("%d\n",num[n]);
        }
    }
}







