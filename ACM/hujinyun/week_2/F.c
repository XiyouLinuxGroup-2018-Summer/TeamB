#include<stdio.h>
#include<string.h>
char map[101][101];
int m,n;
int cur;
void dfs(int x, int y)
{
    if(x<0 || x>=m || y<0 || y>=n || map[x][y]!='@') return;
    else{
        map[x][y]='*';
        dfs(x+1,y);
        dfs(x+1,y-1);
        dfs(x+1,y+1);
        dfs(x,y+1);
        dfs(x,y-1);
        dfs(x-1,y);
        dfs(x-1,y+1);
        dfs(x-1,y-1);
    }
}
int main(void)
{
    int i,j;
    while(scanf("%d %d",&m,&n)!=EOF)
    {
        cur=0;
        if(m==0 && n==0) break;
        for(i=0; i<m; i++)
    {
        getchar();
        for(j=0; j<n; j++)
        scanf("%c",&map[i][j]);
     }     
       for(i=0; i<m; i++)
       for(j=0; j<n; j++)
        {
            if(map[i][j] == '@')
            {
            dfs(i,j);
            cur++;
            }
        }
        printf("%d\n",cur);
    }
    return 0;
}