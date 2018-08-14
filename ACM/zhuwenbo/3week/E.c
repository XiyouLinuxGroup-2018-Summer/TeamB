#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(void)
{
	int num[100][100];
	int dp[100][100];
	int n, m;
	scanf("%d%d", &m, &n);
	int i, j;
	while(n != -1 || m != -1)
	{
		memset(dp, 0, sizeof(dp));
		dp[0] = num[0][0];
		for(i = 0; i < m; i++)
		{
			for(j = 0; j < n; j++)
				scanf("%d", &num[i][j]);
		}
		for(i = 0; i < n; i++)
		{
			for(j = 0; j < n; j++)
			{
				if(num[i][m * j] > num[i][j] && m * j < n)
				{
					if(dp[i][j * m] < dp[i][j] + num[i][j * m])
						dp[i][j * m] = dp[i][j] + num[i][j * m];
				}
				
			}
		}
	}
	return 0;
}
