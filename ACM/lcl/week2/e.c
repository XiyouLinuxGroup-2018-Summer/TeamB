/*************************************************************************
	> File Name: e.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月05日 星期日 19时26分55秒
 ************************************************************************/
#include<stdio.h>
#include<string.h>
int num=0;
struct stu
{
    int S;
    int M;
    int N;
    int pre;
}STU[10000];
void output(int a)
{
    if(STU[a].pre!=-1)
    {
        output(STU[a].pre);
        num++;
    }
}
int visit[102][102]={0};
int find(int front)
{
    if((STU[front].S==0)&&(STU[front].N==STU[front].M))
        return 1;
    else if((STU[front].N==0)&&(STU[front].M==STU[front].S))
        return 1;
    else if((STU[front].M==0)&&(STU[front].N==STU[front].S))
        return 1;
    else 
        return 0;
}
int main(void)
{
    int S1,N1,M1;
    int m,n,s;
    int front=0;
    int rear=1;
    while(1)
    {
        int t;
        front=0;
        rear=1;
        num=0;
        memset(visit,0,sizeof(int)*102*102);
        memset(STU,0,sizeof(struct stu)*10000);
        scanf("%d%d%d",&s,&n,&m);
        if(s==0&&m==0&&n==0)
            break;
        if(s%2!=0)
            printf("NO\n");
        else
        {
            STU[0].S=s;
            STU[0].N=0;
            STU[0].M=0;
            STU[0].pre=-1;
            while(front<rear)
            {
                if((STU[front].S!=0)&&(STU[front].N!=n))//s->n
                {
                    if((STU[front].S+STU[front].N)>n)
                    {
                        N1=n;
                        S1=STU[front].S+STU[front].N-n;
                        M1=s-N1-S1;
                        if(((N1==0)&&(M1==0)&&(S1==s))||(visit[N1][M1]==1))
                            ;
                        else
                        {
                                visit[N1][M1]=1;
                                STU[rear].N=N1;
                                STU[rear].S=S1;
                                STU[rear].M=M1;
                                STU[rear].pre=front;
                                t=find(rear);
                                if(t==1)
                                {
                                    output(rear);
                                    printf("%d\n",num);
                                    break;
                                }
                                rear++;
                        }
                    }
                    else
                    {
                        N1=STU[front].S+STU[front].N;
                        S1=0;
                        M1=s-N1-S1;
                        if(((N1==0)&&(M1==0)&&(S1==s))||(visit[N1][M1]==1))
                            ;
                        else
                        {
                            visit[N1][M1]=1;
                            STU[rear].S=S1;
                            STU[rear].M=M1;
                            STU[rear].N=N1;
                            STU[rear].pre=front;
                            t=find(rear);
                            if(t==1)
                            {
                                output(rear);
                                printf("%d\n",num);
                                break;
                            }
                            rear++;
                        }
                    }
                }
                if((STU[front].S!=0)&&(STU[front].M!=m))//s->m
                {
                    if((STU[front].S+STU[front].M)>m)
                    {
                        M1=m;
                        S1=STU[front].S+STU[front].M-m;
                        N1=s-S1-M1;
                        if(((N1==0)&&(M1==0)&&(S1==s))||(visit[N1][M1]==1))
                            ;
                        else
                        {
                            visit[N1][M1]=1;
                            STU[rear].N=N1;
                            STU[rear].S=S1;
                            STU[rear].M=M1;
                            STU[rear].pre=front;
                            t=find(rear);
                            if(t==1)
                            {
                                output(rear);
                                printf("%d\n",num);
                                break;
                            }
                            rear++;
                        }
                    }
                    else
                    {
                        M1=STU[front].S+STU[front].M;
                        S1=0;
                        N1=s-S1-M1;
                        if(((N1==0)&&(M1==0)&&(S1==s))||(visit[N1][M1]==1))
                            ;
                        else
                        {
                            visit[N1][M1]=1;
                            STU[rear].N=N1;
                            STU[rear].S=S1;
                            STU[rear].M=M1;
                            STU[rear].pre=front;
                            t=find(rear);
                            if(t==1)
                            {
                                output(rear);
                                printf("%d\n",num);
                                break;
                            }
                            rear++;
                        }
                    }
                }
                if((STU[front].N!=0))//n->s
                {
                    S1=STU[front].N+STU[front].S;
                    N1=0;
                    M1=s-S1-N1;
                    if(((N1==0)&&(M1==0)&&(S1==s))||(visit[N1][M1]==1))
                            ;
                    else
                    {
                        visit[N1][M1]=1;
                        STU[rear].N=N1;
                        STU[rear].S=S1;
                        STU[rear].M=M1;
                        STU[rear].pre=front;
                        t=find(rear);
                        if(t==1)
                        {
                            output(rear);
                            printf("%d\n",num);
                            break;
                        }
                        rear++;
                    }
                }
                if((STU[front].N!=0)&&(STU[front].M!=m))//n->m
                {
                    if((STU[front].N+STU[front].M)>m)
                    {
                        M1=m;
                        N1=STU[front].N+STU[front].M-m;
                        S1=s-M1-N1;
                        if(((N1==0)&&(M1==0)&&(S1==s))||(visit[N1][M1]==1))
                            ;
                        else
                        {
                            visit[N1][M1]=1;
                            STU[rear].N=N1;
                            STU[rear].S=S1;
                            STU[rear].M=M1;
                            STU[rear].pre=front;
                            t=find(rear);
                            if(t==1)
                            {
                                output(rear);
                                printf("%d\n",num);
                                break;
                            }
                            rear++;
                        }
                    }
                    else
                    {
                        M1=STU[front].N+STU[front].M;
                        N1=0;
                        S1=s-M1-N1;
                        if(((N1==0)&&(M1==0)&&(S1==s))||(visit[N1][M1]==1))
                            ;
                        else
                        {
                            visit[N1][M1]=1;
                            STU[rear].N=N1;
                            STU[rear].S=S1;
                            STU[rear].M=M1;
                            STU[rear].pre=front;
                            t=find(rear);
                            if(t==1)
                            {
                                output(rear);
                                printf("%d\n",num);
                                break;
                            }
                            rear++;
                        }
                    }
                }
                if((STU[front].M!=0))//m->s
                {
                    S1=STU[front].M+STU[front].S;
                    M1=0;
                    N1=s-S1-M1;
                    if(((N1==0)&&(M1==0)&&(S1==s))||(visit[N1][M1]==1))
                            ;
                    else
                    {
                        visit[N1][M1]=1;
                        STU[rear].N=N1;
                        STU[rear].S=S1;
                        STU[rear].M=M1;
                        STU[rear].pre=front;
                        t=find(rear);
                            if(t==1)
                            {
                                output(rear);
                                printf("%d\n",num);
                                break;
                            }
                        rear++;
                    }
                }
                if((STU[front].M!=0)&&(STU[front].N!=n))//m->n
                {
                    if((STU[front].M+STU[front].N)>n)
                    {
                        N1=n;
                        M1=STU[front].M+STU[front].N-n;
                        S1=s-M1-N1;
                        if(((N1==0)&&(M1==0)&&(S1==s))||(visit[N1][M1]==1))
                            ;
                        else
                        {
                            visit[N1][M1]=1;
                            STU[rear].N=N1;
                            STU[rear].S=S1;
                            STU[rear].M=M1;
                            STU[rear].pre=front;
                            t=find(rear);
                            if(t==1)
                            {
                                output(rear);
                                printf("%d\n",num);
                                break;
                            }
                            rear++;
                        }
                    }
                    else
                    {
                        N1=STU[front].M+STU[front].N;
                        M1=0;
                        S1=s-M1-N1;
                        if(((N1==0)&&(M1==0)&&(S1==s))||(visit[N1][M1]==1))
                            ;
                        else
                        {
                            visit[N1][M1]=1;
                            STU[rear].N=N1;
                            STU[rear].S=S1;
                            STU[rear].M=M1;
                            STU[rear].pre=front;
                            t=find(rear);
                            if(t==1)
                            {
                                output(rear);
                                printf("%d\n",num);
                                break;
                            }
                            rear++;
                        }
                    }
                }
                front++;
            }
            if(rear==front)
                printf("NO\n");
        }
    }
}

