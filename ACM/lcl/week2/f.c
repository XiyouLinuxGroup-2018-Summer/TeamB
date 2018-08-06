/*************************************************************************
	> File Name: f.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月05日 星期日 19时27分44秒
 ************************************************************************/
#include<stdio.h>
#include<string.h>
struct stu
{
    int x;
    int y;
}STU[10000];
int dx[8]={1,1,0,-1,-1,-1,0,1};
int dy[8]={0,1,1,1,0,-1,-1,-1};
int main(void)
{
    int next_x;
    int next_y;
    int i,j,k;
    int n,m;
    while(1)
    {
        int num=0;
        int front=0;
        int rear=1;
        scanf("%d%d",&n,&m);
        getchar();
        if(n==0&&m==0)
            break;
        else
        {
            char path[n][m];
            memset(STU,0,sizeof(struct stu)*100);
            memset(path,'\0',n*m);
            for(i=0;i<n;i++)
            {
                for(j=0;j<m;j++)
                {
                    scanf(" %c",&path[i][j]);
                }
            }
            for(i=0;i<n;i++)
            {
                for(j=0;j<m;j++)
                {
                    if(path[i][j]=='@')
                    {
                        STU[0].x=i;
                        STU[0].y=j;
                        path[i][j]='*';
                        while(front<rear)
                        {
                            for(k=0;k<8;k++)
                            {
                                next_x=STU[front].x+dx[k];
                                next_y=STU[front].y+dy[k];
                                if((next_x>=0)&&(next_x<n)&&(next_y>=0)&&(next_y<m)&&(path[next_x][next_y]=='@'))
                                {
                                    path[next_x][next_y]='*';
                                    STU[rear].x=next_x;
                                    STU[rear].y=next_y;
                                    rear++;
                                }
                            }
                            front++;
                        }
                        memset(STU,0,sizeof(struct stu)*100);
                        front=0;
                        rear=1;
                        num+=1;
                    }               
                }
            }
        }
        printf("%d\n",num);
    }
}

