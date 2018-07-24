#include<stdio.h>
int main()
{
	int m, n;
	int i, j;
	int num[100] = {0};
	scanf("%d%d", &m, &n);
	while(m != 0 || n != 0)
	{
		for(i = 0; i < m; i++)
		{
			scanf("%d", &num[i]);
		}
		for(i = 0; i < m; i++)
		{
			if(n <= num[i])
			{
				for(j = m - 1; j > i - 1; j--)
				{
					num[j + 1] = num[j];
				}
				num[i] = n;
				break;
			}
			if(i == m - 1)
				num[i + 1] = n;
		}
		for(i = 0; i < m + 1; i++)
		{
			printf("%d", num[i]);
			if(i != m)
				printf(" ");
		}
		printf("\n");
		scanf("%d%d", &m, &n);
	}
}
