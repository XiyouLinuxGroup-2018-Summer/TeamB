/*************************************************************************
	> File Name: d.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月12日 星期日 23时03分23秒
 ************************************************************************/
#include<stdio.h>
#include<string.h>
int dp[1001]={0};
int main(void)
{
    int n;
    int num[1001];
    int i,j,max;
    while(scanf("%d",&n)!=EOF)
    {
        memset(num,0,sizeof(int)*1001);
        memset(dp,0,sizeof(int)*1001);
        for(i=0;i<n;i++)
            scanf("%d",&num[i]);
        dp[0]=1;
        for(i=1;i<n;i++)
        {
            dp[i]=1;
            for(j=0;j<i;j++)
            {
                if((num[j]<num[i])&&((dp[j]+1)>dp[i]))
                {
                    dp[i]=dp[j]+1;
                }
            }
        }
        max=dp[0];
        for(i=1;i<n;i++)
        {
            if(dp[i]>max)
                max=dp[i];
        }
        printf("%d\n",max);
    }
}

