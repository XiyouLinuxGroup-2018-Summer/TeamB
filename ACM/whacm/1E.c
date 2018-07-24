#include<stdio.h>
int main(void)
{
	int m;
	scanf("%d",&m);
	while(getchar() != '\n');
	double num[m];
	char c;
	int a,b,i;
	for( i = 0;i < m;i++) {
		scanf("%c %d %d",&c,&a,&b);
		while(getchar() != '\n');
		switch(c) {
			case '+':
				num[i] = a + b;
				break;
			case '-':
				num[i] = a - b;
				break;
			case '*':
				num[i] = a * b;
				break;
			case '/':
				num[i] = (double)a/b;
				break;
		}
	}
	for( i = 0;i < m;i++)
		if(num[i] - (int)num[i] != 0)
			printf("%.2lf\n",num[i]);
		else
			printf("%.0lf\n",num[i]);
	return 0;
}
