/*************************************************************************
	> File Name: c.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月12日 星期日 23时03分05秒
 ************************************************************************/
#include<stdio.h>
#include<string.h>
int dp[1001];
int main(void)
{
    int n;
    int num[1001];
    int i,j,max;
    while(scanf("%d",&n)&&n!=0)
    {
        memset(num,0,sizeof(int)*1001);
        memset(dp,0,sizeof(int)*1001);
        for(i=0;i<n;i++)
            scanf("%d",&num[i]);
        dp[0]=num[0];
        for(i=1;i<n;i++)
        {
            dp[i]=num[i];
            for(j=0;j<i;j++)
            {
                if((num[j]<num[i])&&((dp[j]+num[i])>dp[i]))
                {
                    dp[i]=dp[j]+num[i];
                }
            }
        }
        max=dp[0];
        for(i=1;i<n;i++)
        {
            if(max<dp[i])
                max=dp[i];
        }
        printf("%d\n",max);
    }
}

