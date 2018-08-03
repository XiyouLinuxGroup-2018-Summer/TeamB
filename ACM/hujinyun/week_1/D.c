#include<stdio.h> 
int fun(int a,int b)
{
    int t;
    int d;
    if(b>a)
    {
        t=a;
        a=b;
        b=t;
    }
    while(b!=0)
    {
        d=b;
        b=a%b;
        a=d;
    }
    return a;
}
int main()
{
    int a,b;
    int n;
    while((scanf("%d %d",&a,&b))!=EOF) 
    {
        n=a*b/fun(a,b);
        printf("%d\n",n);
    }
    return 0;
}
