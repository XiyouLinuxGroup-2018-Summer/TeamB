/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月05日 星期日 19时22分05秒
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
int visit[5][5]={0};
int dx[4]={1,0,-1,0};
int dy[4]={0,1,0,-1};
int num[5][5];
struct stu
{
    int x;
    int y;
    int pre;
}path[1000];
void output(int a)
{
    if(path[a].pre!=-1)
    {
        output(path[a].pre);
        printf("(%d, %d)\n",path[a].x,path[a].y);
    }
    else
    {
        printf("(0, 0)\n");
    }
}
void find_path(void)
{
    int i,flag=0;
    int next_x,next_y;
    int front=0;
    int rear=1;
    path[0].x=0;
    path[0].y=0;
    path[0].pre=-1;
    while(front<rear)
    {
        for(i=0;i<4;i++)
        {
            next_x=path[front].x+dx[i];
            next_y=path[front].y+dy[i];
            if((num[next_x][next_y]==0)&&(visit[next_x][next_y]==0)&&(next_x>=0)&&(next_x<5)&&(next_y>=0)&&(next_y<5))
            {
                visit[next_x][next_y]=1;
                path[rear].x=next_x;
                path[rear].y=next_y;
                path[rear].pre=front;
                rear++;
            }
            if((next_x==4)&&(next_y==4))
            {
                output(rear-1);
                flag=1;
                break;
            }
        }
        if(flag==1)
            break;
        front++;
    }
}
int main(void)
{
    int i=0,j=0;
    for(i=0;i<5;i++)
    {
        for(j=0;j<5;j++)
            scanf("%d",&num[i][j]);
    }
    find_path();
}

