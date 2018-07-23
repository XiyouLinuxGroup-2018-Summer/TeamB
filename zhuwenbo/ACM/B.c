#include<stdio.h>
// #include<math.h>
#define PI 3.1415927
int main()
{
	double radit;
	double size;
	double num;
	num = 4.0 / 3;
	while(scanf("%lf", &radit) != EOF)
	{
		size = num * PI * radit * radit * radit;
		printf("%.3lf\n", size);
		size = 0;
	}
}
