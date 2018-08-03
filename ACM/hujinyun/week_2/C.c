#include<stdio.h>
#include<string.h>
int n,k;
int num;
char a[10][10];
int visit[10];

void dfs(int row, int cur)//row表示第几行,cur表示当前放了多少个棋子
{
    
    int i;
    if(cur == k) 
    {num++;return;}
    if(row>n ||(cur +n+1<k+row) )return ;
    for(i=0;i<n;i++)
    if((visit[i]==0) && a[row][i]=='#')
    {
        visit[i] = 1;
        dfs(row+1,cur+1);
        visit[i] = 0;
    }
    dfs(row+1,cur);

}

int main(void)
{
    int i,j;
    while(scanf("%d %d",&n,&k)!=EOF)
    {
        num=0;
        if(n==-1 && k==-1)
        break;
        for(i=0;i<n;i++)
        scanf("%s",&a[i]);

        dfs(0,0);
        printf("%d\n",num);
    }
    return 0;
}