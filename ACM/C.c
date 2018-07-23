#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int f(char *p)
{
	int len, flag;
	int i;
	len = strlen(p) + 1;
	char temp[100] = {0};
	//temp = (char *)malloc(len);
	for(i = 0; i < len - 1; i++)
	{
		temp[i] = p[len - 2 - i];
	}
	flag = strcmp(temp, p);
	if(flag == 0)
		return 1;
	else
		return 0;
}

int main()
{
	char *p;
	int N;
	int res;
	scanf("%d", &N);
	while(N --)
	{
		scanf("%s", p);
		res = f(p);
		if(res == 1)
			printf("yes\n");
		else
			printf("no\n");
	}
}
