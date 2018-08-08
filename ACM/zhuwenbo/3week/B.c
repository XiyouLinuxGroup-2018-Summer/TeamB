#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(void)
{
	int num;
	int count = 0;
	int i;
	while(scanf("%d", &num) != EOF)
	{
		for(i = 0; i <= num / 2; i++)
		{
			count += (num - 2 * i) / 3 + 1;
		}
		printf("%d\n", count);
		count = 0;
	}
	return 0;
}
