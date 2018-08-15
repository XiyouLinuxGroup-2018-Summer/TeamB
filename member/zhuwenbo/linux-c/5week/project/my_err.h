#include<stdio.h>
#include<stdlib.h>

void my_err(char *str, int line)
{
	fprintf(stderr, "line:%d\n", line);
	perror(str);
	exit(0);
}
