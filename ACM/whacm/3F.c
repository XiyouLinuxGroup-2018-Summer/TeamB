#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int num[2001] = {0};
char *fun(int num)
{
	int i;
	char *buf = (char *)malloc(sizeof(char) * 3);
	buf[0] = num/10 + '0';
	buf[1] = num%10 + '0';
	buf[2] = '\0';
	return buf;
}
int main(void)
{
	int N,K;
	scanf("%d",&N);
	int i,j;
	for(i = 0;i < N;i++) {
		scanf("%d",&K);
		int ar[K];
		int br[K-1];
		for(j = 0;j < K;j++)
			scanf("%d",&ar[j]);
		for(j = 0;j < K-1;j++)
			scanf("%d",&br[j]);
		for(j = 0;j < K;j++)
			num[j] = 0;
		
		num[0] = ar[0];
		num[1] = (br[0]>ar[0]+ar[1])?(ar[0]+ar[1]):br[0];
		for(j = 2;j < K;j++) {
			if((num[j-2] + br[j-1]) > num[j-1] + ar[j])
				num[j] = num[j-1] + ar[j];
			else
				num[j] = num[j-2] + br[j-1];
		}
				
		int hour,mintes,second;
		hour = 8;
		mintes = 0;
		second = 0;
		int flag;
		while(num[K-1] >= 3600) {
			hour++;
			num[K-1] -= 3600;
		}
		while(num[K-1] >= 60) {
			mintes++;
			num[K-1] -= 60;
		}
		while(num[K-1] > 0) {
			second++;
			num[K-1]--;
		}
		if(hour >= 24)
			hour-=24;
		if(hour >= 12)
			flag = 1;
		else
			flag = 0;
		char *p1 = fun(hour);
		char *p2 = fun(mintes);
		char *p3 = fun(second);
		switch(flag) {
			case 1:
				printf("%s:%s:%s pm\n",p1,p2,p3);
				break;
			case 0:
				printf("%s:%s:%s am\n",p1,p2,p3);
				break;
		}
	}

	return 0;
}

