#include<stdio.h>
int main(void)
{
	int m,n;

	int ar[200001] = {0};
	long i;
	int cnt = 2;
	scanf("%d %d",&m,&n);
	ar[m] = cnt;
	if(m == n) {
		printf("0\n");
		return 0;
	}
	if(m > n) {
		printf("%d\n",m-n);
		return 0;
	}
	while(1) {
		if(m > 100000)
			m = 100000;
		for(i = 0;i <= m;i++) {
			if(ar[n] != 0)
				break;
			if(ar[i] == cnt) {
				if(ar[i+1] == 0) {
					ar[i+1] = cnt+1;
				}
				if(ar[i*2] == 0) {
					ar[i*2] = cnt+1;
				}
				if(ar[i-1] == 0 && i-1 > 0) {
					ar[i-1] = cnt+1;
				}
				if(ar[n] != 0)
					break;
			}
		}
		cnt++;
		m = 2*i;
		if(ar[n] != 0)
			break;
	}
	printf("%d\n",cnt-2);
	return 0;
}
