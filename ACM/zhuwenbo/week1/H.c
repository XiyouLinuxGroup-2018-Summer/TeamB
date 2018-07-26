#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int fun(char *str)
{
	int len;
	int i;
	len = strlen(str);
	if(!((str[0] >= 'A'&& str[0] <= 'Z') || (str[0] >= 'a' && str[0] <= 'z') || str[0] == '_'))              //判断第一个字符是否为字母
	{	
		return -1;
	}
	for(i = 1; i < len; i++)
	{
		if(!((str[i] >= 'A'&& str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= '0' && str[i] <= '9') || str[i] == '_'))
		{
			return -1;	
		}
	}
	return 1;
}
int main()
{
	int i;
	int N;
	int flag;
	scanf("%d", &N);
	char str[51] = {0};
	while(N--)
	{
	//	memset(str,0,sizeof(str));
		getchar();
		scanf("%[^\n]", str);
		flag = fun(str);
		if(flag == 1)
			printf("yes\n");
		if(flag == -1)
			printf("no\n");
	}
}
