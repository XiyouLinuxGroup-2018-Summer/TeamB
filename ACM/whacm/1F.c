#include<stdio.h>
#include<string.h>
int main(void)
{
	int m,i,j,count;
	scanf("%d",&m);
	while(getchar() != '\n');
	char ptr[m][50];
	int num[m];

	for(i = 0;i < m;i++) {
		gets(ptr[i]);
		count = 0;
		for(j = 0;j < strlen(ptr[i]);j++)
			if(ptr[i][j] >= '0' && ptr[i][j] <= '9')
				count++;
		num[i] = count;
	}

	for(i = 0;i < m;i++)
		printf("%d\n",num[i]);
	return 0;
}
