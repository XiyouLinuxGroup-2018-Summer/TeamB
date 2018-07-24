#include<stdio.h>
#include<string.h>
int main(void)
{
	char ar[100];
	while(gets(ar) ) {
		
		int i;
		ar[0] = ar[0] - 32;
		for(i = 1;i < strlen(ar);i++)
			if(ar[i] == ' ')
				ar[i+1] =ar[i+1] - 32;
		puts(ar);
	}
	return 0;
}
