#include<stdio.h>
int max = 0;
int display(int n,int *ar)
{
    int num[1001];
	int i,j;
	for(i = 0;i < n;i++)
		num[i] = 1;

    for(i = 0;i < n;i++) {
		for(j = 0;j <= i;j++) {
            if(ar[j] < ar[i] && num[j] + 1 > num[i])
                   num[i]++;
        }
    }
    
	int max = 0;
    for(i = 0;i < n;i++)
        if(max < num[i])
            max = num[i];
    return max;
	
}
int main(void)
{
    int n;
	int i,j;
	while(scanf("%d",&n) != EOF && n != 0) {
		int ar[n];
		for(i = 0;i < n;i++) 
			scanf("%d",&ar[i]);

		int num = display(n,ar);
		printf("%d\n",num);
	}
	return 0;
}
