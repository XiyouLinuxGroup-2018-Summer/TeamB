#include<stdio.h>
int add(float a,float b,float c)
{
    if( (a+b)>c )
    return 1;
    else
    return 0;
}
int less(float a, float b, float c)
{
    if( (a-b)<c)
    return 1;
    else
    return 0;
}
int main()
{
    int n;
    float a,b,c;
    scanf("%d",&n);
    while(n--)
    {
        scanf("%f %f %f",&a,&b,&c);
        if(add(a,b,c) && add(a,c,b) && add(b,c,a) && less(a,b,c) && less(a,c,b) && less(b,c,a))
        {
            printf("YES\n");
        }
        else
        printf("NO\n");
    }
}