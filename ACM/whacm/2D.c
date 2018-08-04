#include<stdio.h>
int count = 1;
void display(unsigned long long num,int m,unsigned long long *sum,int cnt)
{
	if(cnt >= 19)
		return;
	if(num%m == 0)	{				
		*sum = num;
		count = 0;
		return;
	}
	if(count) {
		
		display(num*10,m,sum,cnt+1);
		display(num*10+1,m,sum,cnt+1);
	}
	
}
int main(void)
{
	int m;
	int cnt = 1;
	unsigned long long num = 1;
	unsigned long long sum;
	while(scanf("%d",&m) && m != 0) {
		num  = 1;
		count = 1;
		display(num,m,&sum,cnt);
			printf("%llu\n",sum);
	}
	return 0;
}
