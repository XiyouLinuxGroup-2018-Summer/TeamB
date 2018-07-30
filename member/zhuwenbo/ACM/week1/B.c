#include<stdio.h>
// #include<math.h>
#define PI 3.1415927
int main()
{
	double radit;
	double size;
	double num;
	num = 4.0 / 3;                         //只有将值初始化为小数做除法才会得到小数
	while(scanf("%lf", &radit) != EOF)
	{
		size = num * PI * radit * radit * radit;
		printf("%.3lf\n", size);
		size = 0;
	}
}
