//时间超限
/*#include<stdio.h>
int main(void)
{
    int n,s;
    int i,j,k;
    while(scanf("%d",&n)!=EOF)
    {
        s=0;
        /*k=n/3;
        for(i=0;i<=k; i++)
        {
            s=s+(n-i*3)/2+1;
        }*/
  /*      for(i=0;i<=n; i++)
        for(j=0; j<=n/2;j++)
        for(k=0;k<=n/3;k++)
        if(i+j*2+k*3 == n)
        s++;
        printf("%d\n",s);
    }
    return 0;

}

#include<stdio.h>
int main(void)
{
    int dp[3][32480];
    int i,j;
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        for(i=0;i<3;i++)
        for(j=0; j<32480; j++)
        dp[i][j]=0;

        for(i=0;i<n; i++)
        dp[1][i]=1;

        for(i=2;i<=3;i++)
        for(j=0;j<=n;j++)
        {
            if(j>=i) dp[i][j] = dp[i-1][j]+dp[i][j-i];
            else dp[i][j] = dp[i-1][j];
        }
        printf("%d\n",dp[3][n]);
    }

    return 0;
}
*/
#include<stdio.h>
int main(void)
{
    int dp[32769];
    int i,j;
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        for(i=0;i<=n; i++)
        dp[i]=0;
        dp[0]=1;

        for(i=1; i<=3; i++)
            for(j=i; j<=n; j++)
            dp[j]+=dp[j-i];
        printf("%d",dp[n]);
    }
}