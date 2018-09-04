#include <stdio.h>
#include <stddef.h>
#include <time.h>
typedef struct A{
	char a;
	char b;
	int c;
}a;
int main(void)
{
	a data = {'a','b','c'};
	printf("%c\n",*((char *)&data + 4));
	return 0;

}
