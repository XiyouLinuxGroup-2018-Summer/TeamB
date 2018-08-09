#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(void)
{
	int c;
	int n;
	int num[5050] = {0};
	int symbal[100] = {0};
	int i, j;
	int temp, sum;
	int count = 0;
	scanf("%d", &c);
	while(c--)
	{
		scanf("%d", &n);
		j = 1;
		temp = n;
		sum = (n + 1) * n / 2;
		while(temp--)
		{
			for(i = 0; i < j; i++)
				scanf("%d", &num[count++]);
			j++;
		}
		j = n - 1;
		while(j--)
		{
			for(i = 0; i < n - 1; i++)
			{
				if(num[sum - 1 - i] + num[sum - 1 - i - n] > num[sum - 2 - i] + num[sum - 1 - i - n])
				{
					symbal[n - 2 - i] = num[sum - 1 - i] + num[sum - 1 -i -n];
					num[sum - 1 -i -n] = symbal[n - 2- i];
				}
				else
				{
					symbal[n - 2 - i] = num[sum - 2 - i] + num[sum - 1 - i - n];
					num[sum - 1 - i - n] = symbal[n - 2- i];
				}
			}
			sum -= n;
			n--;
		}
		if(n == 1)
		{
			symbal[0] = num[0];
		}
		count = 0;
		printf("%d\n", symbal[0]);
		memset(num, 0, sizeof(num));
		memset(symbal, 0, sizeof(symbal));
	}
	return 0;
}
