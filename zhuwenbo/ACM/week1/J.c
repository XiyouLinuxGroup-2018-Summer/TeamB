#include<stdio.h>
int main()
{
	int a, b;
	int n;
	int i;
	int sum1 = 0, sum2 = 0;
	scanf("%d", &n);
	while(n--)
	{
		scanf("%d%d", &a, &b);
		for(i = 1; i < a; i++)
		{
			if(a % i == 0)
			{
				sum1 += i;
			}
		}
		if(sum1 == b)
		{
			for(i = 1; i < b; i++)
			{
				if(b % i == 0)
				{
					sum2 += i;
				}
			}
		}
		if(sum2 == a)
			printf("YES\n");
		else 
			printf("NO\n");
		sum1 = 0, sum2 = 0;
	}
}
