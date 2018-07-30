#include<stdio.h>

void swap(int * a , int * b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

int qsort(int a[], int low, int high)
{
	int p;
	p = a[low];
	while(low < high)
	{
		while(low < high && p < a[high])
			high--;
		swap(&a[low], &a[high]);
		while(low < high && p > a[low])
			low++;
		swap(&a[low], &a[high]);
	}
	return low;
}

void sort(int a[], int low, int high)
{
	int n;
	if(low < high)
	{
		n = qsort(a, low, high);
		sort(a, low, n - 1);
		sort(a, n - 1, high);
	}
}
int main()
{
	int M;
	int num[3];
	scanf("%d", &M);
	while(M--)
	{
		scanf("%d%d%d", &num[0], &num[1], &num[2]);
		sort(num, 0, 2);
		if(num[0] + num[1] > num[2] && num[2] - num[0] < num[1])
		{
			printf("YES\n");
		}
		else
			printf("NO\n");
	}
}
