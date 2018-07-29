#include<stdio.h>
int main(void)
{
	char a,b,c;
	while(scanf("%c %c %c",&a,&b,&c) != EOF) {
		while(getchar() != '\n');
		printf("%c %c %c\n",(a<b)?((a<c)?a:c):((c<b)?c:b),(a>b)?((b>c)?b:((a>c)?c:a)):((c<a)?a:((b>c)?c:b)),(a>b)?((a>c)?a:c):((c>b)?c:b));
	}
	return 0;
}
