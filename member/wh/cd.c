#include<stdio.h>
#include<unistd.h>
int main(int argc,char *argv[])
{
	if(argc > 2) {
		printf("error");
		return 0;
	}
	chdir("argv[1]");
	return 0;
}
