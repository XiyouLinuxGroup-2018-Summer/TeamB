#include<stdio.h>
int max(int x, int y)
{
    return x>y ? x : y;
}
int main(void)
{
    int result[101];
    int a[101];
    int n;
    int ant;
    int i,j;
    while(scanf("%d",&n)!=EOF)
    {
        for(i=1; i<=n;i++)
        {
            scanf("%d",&a[i]);
            result[i] = a[i];
        }
        result[0]=0;
        
        for(i=1;i<=n;i++)
            for(j=1;j<=n;j++)
            {
                if(a[j]>a[i] && j>i)
            //    result[j] = result[i]+a[j];
                result[j] = max(result[i] + a[j],result[j]);
                
                else
                result[j]=max(a[j],result[j]);  
            }
            ant = -999999;
            for(i=1; i<=n; i++) //记得数组是从1开始的
            if(result[i]>ant)
            ant=result[i];

            printf("%d\n",ant);
    }
}
/*
#include<stdio.h>
int main(void)
{
    int result[1001];
    int a[1001];
    int n;
    int ant;
    int i,j;
    while(scanf("%d",&n)!=EOF)
    {
        for(i=1; i<=n;i++)
        {
            scanf("%d",&a[i]);
            result[i] = a[i];
        }
        result[0]=0;
        
        for(i=1;i<=n;i++)
            for(j=1;j<=n;j++)
            {
                if(a[j]>a[i] && j>i)
                result[j] = result[i]+a[j];
            }
            ant = -999999;
            for(i=1; i<=n; i++) //记得数组是从1开始的
            if(result[i]>ant)
            ant=result[i];

            printf("%d\n",ant);
    }
}*/