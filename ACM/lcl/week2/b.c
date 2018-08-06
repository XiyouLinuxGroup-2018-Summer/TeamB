/*************************************************************************
	> File Name: b.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月05日 星期日 19时22分20秒
 ************************************************************************/
#include<stdio.h>
#include<string.h>
struct stu
{
    int x;
    int pre;
}path[2000000];
int num=0;
int visit[100001]={0};
void output(int a)
{
    if(path[a].pre!=-1)
    {
        output(path[a].pre);
        num+=1;
    }
}
int main(void)
{
    int a,b;
    int next_x;
    int front;
    int rear;
    while((scanf("%d%d",&a,&b))!=EOF)
    {
        if(a==b)
        {
            printf("0\n");
            continue;
        }
        memset(visit,0,100001*sizeof(int));
        memset(path,0,sizeof(struct stu)*2000000);
        num=0;
        front=0;
        rear=1;
        path[front].x=a;
        visit[a]=1;
        path[front].pre=-1;
        while(front<rear)
        {
            next_x=path[front].x+1;
            if((visit[next_x]==0)&&(next_x<=100000))
            {
                visit[next_x]=1;
                path[rear].x=next_x;
                path[rear].pre=front;
                rear+=1;
            }
            if(next_x==b)
            {
                output(rear-1);
                printf("%d\n",num);
                break;
            } 
            next_x=path[front].x-1;
            if((next_x>=0)&&(visit[next_x]==0)&&(next_x<=100000))
            {
                visit[next_x]=1;
                path[rear].x=next_x;
                path[rear].pre=front;
                rear+=1;
            }
            if(next_x==b)
            {
                output(rear-1);
                printf("%d\n",num);
                break;
            }
            next_x=(path[front].x)*2;
            if((next_x<100000)&&(visit[next_x]==0))
            {
                visit[next_x]=1;
                path[rear].x=next_x;
                path[rear].pre=front;
                rear+=1;
            }
            if(next_x==b)
            {
                output(rear-1);
                printf("%d\n",num);
                break;
            }
            front+=1;
        }
    }
}

