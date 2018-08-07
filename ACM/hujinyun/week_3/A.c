#include<stdio.h>
int main(void)
{
    int m,n;
    int i,j;
    int a[101][101];
    scanf("%d",&m);
    while(m--)
    {
        scanf("%d",&n);
        if(n>0 && n<101)
        {
                for(i=0; i<n; i++)
                    for(j=0; j<i+1; j++)
                    {
                        scanf("%d",&a[i][j]);
                    }
                for(i=0; i<n; i++)
                {
                        for(j=0; j<i+1; j++)
                            printf(" %d",a[i][j]);
                        printf("\n");
                }
                for(i=n-1; i>0; i--)
                    for(j=0; j<i; j++)
                    {
                        if(a[i][j] > a[i][j+1])
                        {
                            a[i-1][j]+=a[i][j];
                        }
                        else
                        {
                            a[i-1][j]+=a[i][j+1];
                        }
                    }
                    printf("%d\n",a[0][0]);
        }
               

    }


    return 0;
}