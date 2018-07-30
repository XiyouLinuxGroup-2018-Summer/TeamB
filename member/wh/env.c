#include<stdio.h>
#include<malloc.h>
int main(int argc,char *argv[],char **envp)
{
	int i,j;
	for(i = 0;envp[i];i++) 
		printf("%s\n",envp[i]);
	return 0;
}
