#include<stdio.h>
int main(void)
{
	int m,n;
	int i,j;	
	while(scanf("%d %d",&m,&n) != EOF) {
		for(i = m;i <= m*n;i+=m) {
			for(j = n;j <= m*n;j+=n)
				if(i == j) {
					printf("%d\n",i);
					break;
				}
			if(i == j)
				break;
		}
	}
	return 0;
}
