/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 15时49分00秒
 ************************************************************************/
//--------C回文串
#include<stdio.h>
#include<string.h>
int main(void)
{
    int n,i,f=0,j;
    char num[100];
    scanf("%d",&n);
    getchar();
    while(n--)
    {
        f=0;
        gets(num);
        i=0;
        j=strlen(num)-1;
        if(strlen(num)%2!=0)
        {
            while(i!=j)
            {
                if(num[i]!=num[j])
                {
                    f=1;
                    break;
                }
                else
                {
                    i++;
                    j--;
                }
            }
            if(f==1)
                printf("no\n");
            else
                printf("yes\n");
        }
        else
        {
            while(i<j)
            {
                if(num[i]!=num[j])
                {
                    f=1;
                    break;
                }
                else
                {
                    i++;
                    j--;
                }
            }
            if(f==1)
                printf("no\n");
            else
            printf("yes\n");
        }
    }
}

