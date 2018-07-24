#include<stdio.h>
int main()
{
	int i, j;
	char str[3];
	while(scanf("%c%c%c", &str[0], &str[1], &str[2]) != EOF)
	{
		getchar();
		for(i = 0; i < 3; i++)
		{
			for(j = i + 1; j < 3; j++)
			{
			    if(str[i] > str[j])
			    {
			    	char temp;
				temp = str[i];
				str[i] = str[j];
				str[j] = temp;
		    	    }
			}
		}
		for(i = 0; i < 3; i++)
		{
			printf("%c", str[i]);
			 if(i != 2)
			    printf(" ");
		}
		printf("\n");
	}
}
