#include<stdio.h> 
#include<math.h>
#define PI 3.1415927
int main()
{
    double r;
    while((scanf("%lf",&r)!=EOF))
    {
        printf("%.3lf\n",PI*r*r*r*4.0/3.0); 
    }
    return 0;
}
