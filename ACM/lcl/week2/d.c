/*************************************************************************
	> File Name: d.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月05日 星期日 19时25分14秒
 ************************************************************************/
#include<stdio.h>
#include<string.h>
struct stu
{
    long long int sum;
    int pre;
}STU[100001];
int main(void)
{
    int flag=0;
    int front,rear;
    int n;
    long long int next_sum;
    while(1)
    {
        flag=0;
        scanf("%d",&n);
        if(n==0)
            break;
        next_sum=1;
        front=0;
        rear=1;
        memset(STU,0,sizeof(struct stu)*100001); 
        STU[front].sum=next_sum;
        STU[front].pre=-1;
        while(front<rear)
        {
            if(rear>=1000000)
                break;
            next_sum=STU[front].sum*10;
            if(next_sum%n==0)
            {
                flag=1;
                printf("%lld\n",next_sum);
                break;
            }
            else if(next_sum<=1111111111111111111)
            {
                STU[rear].sum=next_sum;
                rear++;
            }
            next_sum=STU[front].sum*10+1;
            if(next_sum%n==0)
            {
                flag=2;
                printf("%lld\n",next_sum);
                break;
            }
            else if(next_sum<=1111111111111111111)
            {
                STU[rear].sum=next_sum;
                rear++;
            }
            front++;
        }
    }
}

