#include<stdio.h>
int convert(const char *num)
{
	int i;
	int sum = 0;
	for(i = 0;num[i] != '\0';i++)
		sum=(num[i] - '0') + sum*10;
	return sum;
}
int main(void)
{
	char *path = "123";
	printf("%d",convert(path));
	return 0;
}
