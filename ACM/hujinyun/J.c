#include<stdio.h>
int fun(int m)
{
    int sum=0;
    int i;
    if(m<=0)
    return 0;
    else if(m==1)
    return 1;
  
    else
    {
        for(i=1; i<m; i++)
        {
            if(m%i==0)
            sum+=i;
        }
        return sum;
    }
    
}
int main()
{
    int n;
    int a,b;
    int p,q;
    scanf("%d",&n);
    while(n--)
    {
        scanf("%d %d",&a,&b);
        p=fun(a);
        q=fun(b);
        if(p==b && q==a)
        printf("YES\n");
        else
        {
            printf("NO\n");
        }
    }
}