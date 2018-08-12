/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月12日 星期日 23时02分18秒
 ************************************************************************/
#include<stdio.h>
#include<string.h>
int main(void)
{
    int group;
    int high;
    int i=0,j=0,k;
    scanf("%d",&group);
    while(group--)
    {
        scanf("%d",&high);
        int num[high][high];
        memset(num,0,high*high);
        for(i=0;i<high;i++)
        {
            for(j=0;j<=i;j++)
                scanf("%d",&num[i][j]);
        }
        for(i=high-1;i>=0;i--)
        {
            for(j=0;j<i;j++)
            {
                if(num[i][j]>num[i][j+1])
                    num[i-1][j]+=num[i][j];
                else
                    num[i-1][j]+=num[i][j+1];
            }
        }
        printf("%d\n",num[0][0]);
    }
}

