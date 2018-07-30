#include<stdio.h>
#include<string.h>

int main()
{
	char str[101];
	int i = 1;
	int len;
	while(scanf("%[^\n]", str) != EOF)
	{
		len = strlen(str);
		if(str[0] >= 'a' && str[0] <= 'z')
			str[0] = str[0] - 32;
		/*	
		while( i < len)
		{
			while(str[i] != ' ' && i < len)
				i++;
			str[i + 1] -= 32;
			i++;
		}
		*/
		for(i = 1; i < len; i++)
		{
			if(str[i] == ' ')
				str[i + 1] -= 32;
		}
		printf("%s\n", str);
		//i = 1;
		getchar();
	}
}
