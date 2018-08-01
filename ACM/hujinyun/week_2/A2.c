#include<stdio.h>
#include<string.h>
int dx[4]={1,-1,0,0};
int dy[4]={0,0,1,-1};
int map[5][5];
int pre[100];
int visit[5][5];

struct str
{
    int x;
    int y;
}q[3000];
void print(int n);
void bfs()
{
    int i,j;
    int front = 0;
    int rear = 1;
    int vx,vy;
    int a,b;
    for(i=0; i<5; i++)//初始化
    for(j=0; j<5; j++)
    visit[i][j] = 0;
    q[0].x = 0;
    q[0].y = 0;
    pre[0] = -1;
    while(front<rear)
    {
        a = q[front].x;
        b = q[front].y;
        if(a == 4 && b == 4)
        {
            print(front);
            return ;
        }

        for(i = 0; i < 4; i++)
        {
            vx = a + dx[i];
            vy = b + dy[i];
            if(visit[vx][vy]==0 && vx>=0 && vx<5 && vy>=0 &&vy<5 && map[vx][vy]==0)
            {
                visit[vx][vy] = 1;
                q[rear].x=vx;
                q[rear].y=vy;
                pre[rear]=front;
                rear++;
            }
        }
        front++;
    }
    return;
}
void print(int n)
{
    int t;
    t=pre[n];
    if(t==0)
    {
        printf("(0, 0)\n");
        printf("(%d, %d)\n",q[n].x,q[n].y);
        return ;
    }
    else
    {
        print(t);
        printf("(%d, %d)\n",q[n].x,q[n].y);
    }
}
int main(void)
{
    int i,j;
    for(i=0; i<5; i++)
    for(j=0; j<5; j++)
    scanf("%d",&map[i][j]);
    bfs();
    return 0;
}