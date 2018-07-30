#include<stdio.h>
int main()
{
	int N;
	double A, B;
	char c[2] = {0};
	scanf("%d", &N);
	while(N--)
	{
		scanf("%s", c);
		scanf("%lf%lf", &A, &B);
		if(c[0] == '+')
		{
			if(A - (int)A == 0 && B - (int)B == 0)          //判断A,B是否为整数
				printf("%.0lf\n", A + B);
			else
				printf("%.2lf\n", A + B);
		}
		if(c[0] =='-')
		{
			if(A - (int)A == 0 && B - (int)B == 0)
				printf("%.0lf\n", A - B);
			else
				printf("%.2lf\n", A - B);
		}
		if(c[0] == '*')
		{
			if(A - (int)A == 0 && B - (int)B == 0)
				printf("%.0lf\n", A * B);
 			else
				printf("%.2lf\n", A * B);
		}
		if(c[0] == '/')
		{
			if((A / B) - (int)(A / B) == 0)
				printf("%.0lf\n", A / B);
			else
				printf("%.2lf\n", A / B);
		}
	}
}
