#include<stdio.h>
int fun(int n)
{
    int i;
    if(n<=1)
    return (-1);
    else if(n == 2)
    return 1;
    else
    {
        for(i=2; i<n; i++)
        {
            if(n%i==0)
            break;
        }
        if(i==n)
        return 1;
        else 
        return (-1); 
    }
}
int main()
{
    int x,y;
    int temp,temp1;
    int i;
    while(scanf("%d %d",&x,&y) && (x!=0 || y!=0) && (x<=y) &&(x>=-39 && x<=50) &&(y>=-39 && y<=50) )
    {
        for(i=x; i<=y; i++)  //范围是否写对（开闭区间）
        {
            temp= i*i+i+41;
            temp1=fun(temp);
            if(temp1 == -1)
            break;
        }
        if(i==(y-x+1))
        printf("Ok\n");        
        else
        printf("sorry\n");       
    }
}