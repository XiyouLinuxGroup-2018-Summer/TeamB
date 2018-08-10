#include<stdio.h>
int max(int x, int y)
{
    return x>y ? x: y;
}
int main(void)
{
    int i,j;
    int n;
    int a[1001];
    int dp[1001];
    while(scanf("%d",&n)!=EOF)
    {
        for(i=1;i<=n; i++)
        {
            scanf("%d",&a[i]);
            dp[i]=1;
        }
        
        int maxn=0;

        for(i=1; i<=n; i++)
        {
             for(j=1; j<=i; j++)
            {
                if(a[j]<a[i])//
                {
                     dp[i]=max(dp[j]+1,dp[i]);
                    
                    printf("a[i]=%d,dp[i]=%d,a[j]=%d,dp[j]=%d   i=%d,j=%d\n",a[i],dp[i],a[j],dp[j],i,j);

                }
            }  
        }  

        for(i=1;i<=n; i++)
            if(maxn<dp[i])
                maxn=dp[i];      
        printf("%d\n",maxn);

    }
}