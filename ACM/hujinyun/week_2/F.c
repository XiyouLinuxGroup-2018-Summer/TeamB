#include<stdio.h>
#include<string.h>
char map[101][101];
int m,n;
int cur;

void dfs(int x, int y)
{
 //   printf("k");
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
//return ;
    }


}
int main(void)
{
    int i,j;
    while(scanf("%d %d",&m,&n)!=EOF)
    {
        cur=0;
        if(m==0 && n==0) break;

        while(getchar()!= '\n');
        for(i=0; i<m; i++)
        for(j=0; j<n; j++)
        scanf("%c",&map[i][j]);  //此处不同行同列不用%s
        while(getchar()!= '\n');

        for(i=0; i<m; i++)
        for(j=0; j<n; j++)
        {
            if(map[i][j] == '@')
            {
              printf("testi j\n");
            dfs(i,j);
            cur++;

            }
        }
        printf("%d\n",cur);
    /*    for(i=0; i<m;i++)
        for(j=0;j<n;j++)
        map[i][j]=0;*/

    }
    return 0;
}