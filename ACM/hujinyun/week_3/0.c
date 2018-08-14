#include<stdio.h>
int fib[101]={0};
/*int f(int x)
{
    if(fib[x]!=0) return fib[x];
    return fib[x]=(x>2)?f(x-1)+f(x-2):1;

}*/
int f(int x)
{
    int i;
    fib[1]=fib[2]=1;
    for(i=3;i<=x; i++)
    {
        fib[i]=fib[i-1]+fib[i-2];
    }
    return fib[x];
}

int main(void)
{
    int n;
    int i;
    while(scanf("%d",&n)!=EOF)
    {
        for(i=0; i<101; i++)
        fib[101]=0;
        printf("%d\n",f(n));
    }
}