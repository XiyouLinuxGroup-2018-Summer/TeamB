#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int convert(const char *name)
{
	int i = 0;
	int flag = 1;               //判断符号
	int num = 0;
	if(name[0] == '-')
		flag = -1;
	for(i = 0; name[i] != '\0'; i++)
	{
		if(name[i] >= '0' && name[i] <= '9')
		{
			num *= 10;
			num += (name[i] - '0');
		}
	}

	num *= flag;
	return num;
}

int main()
{
	const char *name = "-aaa a12313123";
	int num = convert(name);
	printf("%d\n", num);	
}
