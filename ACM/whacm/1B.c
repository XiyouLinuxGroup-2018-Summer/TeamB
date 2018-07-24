#include<stdio.h>
int main(void)
{
	double r;
	while(scanf("%lf",&r) != EOF)
		printf("%.3f\n",(4.0*3.1415927*r*r*r)/3.0);
	return 0;
}
