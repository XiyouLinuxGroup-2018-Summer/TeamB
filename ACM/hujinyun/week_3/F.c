#include<stdio.h>
int min(int x, int y)
{
    return x<y ? x: y;
}

int main(void)
{
    int C;
    int N;
    int a[1001];
    int b[1001];
    int dp[1001];
    int i,j;
    scanf("%d",&C);
    if(C<=0 || C>10) return 0;
    else
    {
        while(C--)
        {
            scanf("%d",&N);
            if(N>=1 && N<=2000)
            {
                for(i=1;i<=N;i++)
                scanf("%d",&a[i]);
            for(i=1; i<=N-1; i++)
                scanf("%d",&b[i]);
            dp[0]=0;
            dp[1]=a[1];
            for(i=2; i<=N; i++)
            {    
                dp[i]=min(dp[i-1]+a[i],dp[i-2]+b[i-1]);
            }
            
          //  printf("%d\n",dp[N]);
            int s=0,m=0,h=8;
            int flag =0;
            s=dp[N];
            m=s/60;
            if(m>=60) {m=m%60; h++;}
            s=s%60;
            if(h>12) {h=h%12; flag=1;}
          //  printf("H:%d M:%d S:%d\n",h,m,s);
            if(h>=0 && h<10)
            printf("0%d",h);
            else 
                printf("%d",h);
            if(m>=0 && m<10)
                printf(":0%d",m);
            else
                printf(":%d",m);
            if(s>=0 && s<10)
                printf(":0%d",s);
            else
                printf(":%d",s);
            if(flag == 0)
                printf(" am\n");
            else
                printf(" pm\n");

            }

            
        }
    }
    
    return 0;
}
/*
#include"stdio.h"
int min(int a,int b)
{
	return a>b?b:a;
}
int main()
{
	int n;
	int k;
	int dp[2010];
	int num1[2010],num2[2010];
	int i;
	int temp;
	int ans1,ans2,ans3,flag;
	scanf("%d",&n);
	while(n--)
	{
		scanf("%d",&k);
		for(i=1;i<=k;i++)	scanf("%d",&num1[i]);
		for(i=2;i<=k;i++)	scanf("%d",&num2[i]);
 
 
		dp[0]=0;
		dp[1]=num1[1];
		for(i=2;i<=k;i++)	dp[i]=min(dp[i-1]+num1[i],dp[i-2]+num2[i]);
 
 
		temp=dp[k]+8*3600;
		ans1=temp/3600;
		temp%=3600;
		ans2=temp/60;
		temp%=60;
		ans3=temp;
 
 
		flag=0;
		if(ans1>=12)	{ans1%=12;flag=1;}
 
 
		printf("%02d:%02d:%02d",ans1,ans2,ans3);
		if(flag)	printf(" pm\n");
		else		printf(" am\n");
	}
	return 0;
}
*/