#include<stdio.h>
#include<string.h>

typedef struct node
{
    int x;
    int step;
}Node;
Node q[1000000]; //该点坐标和走过步数
int visit[200000]; //该点坐标和走过步数
int a,b;//起始点与终点

int bfs(void)
{
    int i;
    int front = 0;
    int rear = 1;

    Node now,next;

    for(i=0; i<200000; i++)
    visit[i]=0;

    q[0].x=a;//将队列里的数清空;
    q[0].step=0;//将visit数组清空
    visit[a]=1;//开始的位置标记,代表他不会回来

    while(front<rear)//队非空
    { 
        now=q[front];//取队首
        front++;//弹出队首
        for(i=0; i<3; i++)
        {
            if(i==0)
            {
                next.x = now.x-1;
            }
            if(i==1)
            {
                next.x = now.x+1;
            }
            if(i==2)
            {
                next.x = now.x*2;
            }
            if( next.x>=0 && next.x<=200000 &&visit[next.x]==0)
            {
                visit[next.x]=1;
                next.step=now.step+1;
                q[rear].x=next.x;
                q[rear].step=next.step;
                rear++;
            }
            if(next.x==b) return(next.step);
        }
    }
}
int main(void)
{
    int step2;
    while(   scanf("%d %d",&a,&b)!=EOF )
    {
        if(a>=b)printf("%d\n",a-b);
        else
        { 
            step2=bfs();
            printf("%d\n",step2);
        }
    }
    return 0;
}
