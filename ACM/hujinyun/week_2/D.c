#include<stdio.h>
#include<string.h>
int n,m;

void dfs(n,m);
{
    if(m%n==0) return;//如何直接回到主函数
    for(i=0;i<2;i++)
    {
        if(i==1)m=m*10;dfs(n,m);
        if(i==2)m=m*10+1; dfs(n,m);
    }
}
int main(void)
{
    while(scanf("%d",&n)!=EOF)
    {
        m=1;
        if(n==0) break;

        dfs();
    }
    return 0;
}