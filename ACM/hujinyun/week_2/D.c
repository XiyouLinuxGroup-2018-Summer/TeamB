#include<stdio.h>
#include<string.h>
int n; //输入值
int flag;

void dfs(long long m, int step)
{
    if(flag==1 || step>18) return;//返回
    if(m%n==0)
    {
        printf("%lld\n",m); //将m值输出
        flag=1;
        return;
    }
    dfs(m*10,step+1);
    dfs(m*10+1,step+1);
}

int main(void)
{
    while(scanf("%d",&n)!=EOF)
    {
        if(n==0) break;
        flag=0; //注意这里要对flag进行更新
        dfs(1,0);
    }
    return 0;
}
