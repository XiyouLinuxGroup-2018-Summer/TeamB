#include<stdio.h>
#include<string.h>
typedef struct node
{
    int s;
    int m;
    int n;
    int step;
}Node;

Node q[101];
int visit[101][101][101];
int s,m,n;
void bfs()
{
    Node now,next;
    int i,j,k;
    for(i=0; i<101; i++)
    for(j=0; j<101; j++)
    for(k=0; k<101; k++)
    visit[i][j][k]=0;
    q[0].s=s;
    q[0].m=0;
    q[0].n=0;
    int front = 0;
    int rear = 1;
    visit[s][m][n]=1;
    while(front < rear)
    {
        now=q[front];
        front++;
        if((now.s==now.m && now.n==0)||(now.s==now.n && now.m==0) ||(now.m==now.n && now.s==0))
        {
            printf("%d\n",now.step);
            return ;
        }
        for(i=0; i<6; i++)
        {
            if(i==0)
            {
                if(now.s<=(m-now.m))
                {
                    next.m=now.m+now.s;
                    next.s=0;
                    next.n=now.n;
                    next.step=now.step+1;
                }
                else
                {
                    next.m=m;
                    next.s=now.s-(m-now.m);
                    next.n=now.n;
                    next.step=now.step+1;
                }
                if(visit[next.s][next.m][next.n]==0)
                {
                    q[rear].s=next.s;
                    q[rear].m=next.m;
                    q[rear].n=next.n;
                    q[rear].step=next.step;
                    rear++;
                    visit[next.s][next.m][next.n]=1;
                }
            }
                else if(i==1)
            {
                if(now.s<=(n-now.n))
                {
                    next.n=now.n+now.s;
                    next.s=0;
                    next.m=now.m;
                    next.step=now.step+1;
                }
                else
                {
                    next.n=n;
                    next.s=now.s-(n-now.n);
                    next.m=now.m;
                    next.step=now.step+1;
                }
                if(visit[next.s][next.n][next.n]==0)
                {
                    q[rear].s=next.s;
                    q[rear].m=next.m;
                    q[rear].n=next.n;
                    q[rear].step=next.step;
                    rear++;
                    visit[next.s][next.n][next.n]=1;
                }
            }
               else if(i==2)
            {
                if(now.m<=(s-now.s))
                {
                    next.s=now.m+now.s;
                    next.m=0;
                    next.n=now.n;
                    next.step=now.step+1;
                }
                else
                {
                    next.s=s;
                    next.m=now.m-(s-now.s);
                    next.n=now.n;
                    next.step=now.step+1;
                }
                if(visit[next.s][next.m][next.n]==0)
                {
                    q[rear].s=next.s;
                    q[rear].m=next.m;
                    q[rear].n=next.n;
                    q[rear].step=next.step;
                    rear++;
                    visit[next.s][next.m][next.n]=1;
                }
            }
               else if(i==3)
            {
                if(now.m<=(n-now.n))
                {
                    next.n=now.n+now.m;
                    next.m=0;
                    next.s=now.s;
                    next.step=now.step+1;
                }
                else
                {
                    next.n=n;
                    next.m=now.m-(n-now.n);
                    next.s=now.s;
                    next.step=now.step+1;
                }
                if(visit[next.s][next.m][next.n]==0)
                {
                    q[rear].s=next.s;
                    q[rear].m=next.m;
                    q[rear].n=next.n;
                    q[rear].step=next.step;
                    rear++;
                    visit[next.s][next.m][next.n]=1;
                }
            }
            else if(i==4)
            {
                if(now.n<=(s-now.s))
                {
                    next.s=now.s+now.n;
                    next.n=0;
                    next.m=now.m;
                    next.step=now.step+1;
                }
                else
                {
                    next.s=s;
                    next.n=now.n-(s-now.s);
                    next.m=now.m;
                    next.step=now.step+1;
                }
                if(visit[next.s][next.m][next.n]==0)
                {
                    q[rear].s=next.s;
                    q[rear].m=next.m;
                    q[rear].n=next.n;
                    q[rear].step=next.step;
                    rear++;
                    visit[next.s][next.m][next.n]=1;
                }
            }
               else if(i==5)
            {
                if(now.n<=(m-now.m))
                {
                    next.m=now.m+now.n;
                    next.n=0;
                    next.s=now.s;
                    next.step=now.step+1;
                }
                else
                {
                    next.m=m;
                    next.n=now.n-(m-now.m);
                    next.s=now.s;
                    next.step=now.step+1;
                }
                if(visit[next.s][next.m][next.n]==0)
                {
                    q[rear].s=next.s;
                    q[rear].m=next.m;
                    q[rear].n=next.n;
                    q[rear].step=next.step;
                    rear++;
                    visit[next.s][next.m][next.n]=1;
                }
            }
           /* if((next.s==next.m&&next.n==0) || (next.s==next.n &&next.m==0)||(next.m==next.n&&next.s==0))
            {
                printf("%d\n",next.step);
            }*/
        }
    }
   printf("NO\n");
}

int main(void)
{
    while(scanf("%d %d %d",&s,&m,&n)!=EOF)
    {
        if(s==0&&m==0&&n==0)
        break;

        if(s%2==1)
        printf("NO\n");
        else
        bfs();
    }
    return 0;
}