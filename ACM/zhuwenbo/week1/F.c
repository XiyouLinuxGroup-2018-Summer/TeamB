#include<stdio.h>
#include<string.h>
int fun(char str[])
{
	int i;
	int sum = 0;
	int len = strlen(str); 
	for(i = 0; i < len; i++)
	{
		if(str[i] >= '0' && str[i] <= '9')
			sum++;
	}
	return sum;
}
int main()
{
	int n;
	int sum;
	char str[100] = {0};
	scanf("%d", &n);
	while(n--)
	{
		scanf("%s", str);
		sum = fun(str);
		printf("%d\n", sum);
		sum = 0;
	}
}
