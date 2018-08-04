#include<stdio.h>
int main(void)
{
	printf("%d\n",&(&4[(int *)3])[5]);			//(4+5)*sizeof(int)+3
	return 0;
}
