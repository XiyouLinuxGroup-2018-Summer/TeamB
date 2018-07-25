/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 15时49分00秒
 ************************************************************************/
//--------I三角形
#include<stdio.h>
int f(double a,double b,double c)
{
    if(((a+b)>c)&&((a+c)>b)&&((b+c)>a)&&((a-b)<c)&&((a-c)<b)&&((b-c)<a))
        return 1;
    else
        return 0;
}
int main(void)
{
    int n;
    double a,b,c;
    scanf("%d",&n);
    while(n--)
    {
        scanf("%lf%lf%lf",&a,&b,&c);
        if(f(a,b,c)==0)
            printf("NO\n");
        else
            printf("YES\n");
    } p=phead->next;
    while(p!=NULL)
    {
        printf("%d\n",p->num);
        p=p->next;
    }
}





