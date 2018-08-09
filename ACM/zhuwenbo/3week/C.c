#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(void)
{
	int n;
	int num[1002] = {0};
	int dp[1002] = {0};
	int i, j;
	int sum = 0;
	scanf("%d", &n);
	while(n != 0)
	{
		for(i = 1; i < n + 1; i++)
			scanf("%d", &num[i]);
		num[0] = 0;
		for(i = 1; i < n + 1; i++)
		{
			for(j = 0; j < i; j++)
			{
				if(num[i] > num[j])
				{
					if(dp[j] + num[i] > dp[i])
						dp[i] = num[i] + dp[j];
				}
			}
		}
		for(i = 1; i < n + 1; i++)
		{
			if(dp[i] > sum)
			{
				sum = dp[i];
			}
			
		}
		printf("%d\n", sum);
		memset(num, 0, sizeof(num));
		memset(dp, 0, sizeof(dp));
		sum = 0;
		scanf("%d", &n);
	}
	return 0;
}
