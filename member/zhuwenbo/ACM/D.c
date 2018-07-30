#include<stdio.h>
int main()
{
	int a, b, c;
	int num1, num2;
	int sum;
	while(scanf("%d%d", &a, &b) != EOF)
	{
		if(a > b)
		{
			num1 = a;
			num2 = b;
		}
		else
		{
			num1 = b;
			num2 = a;
		}
		if(num1 % num2 == 0)
		{
			sum = a * b / num2;
			printf("%d\n", sum);
		}
		else
		{
			c = num1 % num2;
			while(c != 0)
			{
				num1 = num2;
				num2 = c;
				c = num1 % num2;
			}
			sum = a * b / num2;
			printf("%d\n", sum);
		}
		sum = 0;
	}	
}
