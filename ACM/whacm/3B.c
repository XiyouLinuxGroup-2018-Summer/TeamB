#include<stdio.h>
int main(void)
{
	long N;
	long cnt = 0;
	int i,j,k;
	while(scanf("%ld",&N)!= EOF) {
	for(i = 0;i <= N/3;i++)
		for(j = 0;j <= (N-3*i)/2;j++)
					cnt++;
	printf("%ld\n",cnt);
	cnt = 0;
	}
	return 0;
}
