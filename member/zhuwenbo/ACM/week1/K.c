#include<stdio.h>
#include<math.h>

int fun(int a)
{
	int flag = 0;
	int i;
	for(i = 2; i < a; i++)
	{
		if(a % i == 0)
		{
			flag = -1;
			break;
		}
	}
	return flag;
}
int main()
{
	int x, y;
	int i;
	int j;
	int temp;
	int flag;
	scanf("%d%d", &x, &y);
	while(x != 0|| y != 0)
	{
		for(i = x; i <= y; i++)
		{
			temp = i * i + i + 41;
			flag = fun(temp);
			if(flag == -1)
				break;
		}
		if(flag == -1)
			printf("Sorry\n");
		else
			printf("OK\n");
		scanf("%d%d", &x, &y);
	}
}
