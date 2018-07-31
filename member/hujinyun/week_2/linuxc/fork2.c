#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
	int k;
	char *string;
	pid_t pid;
	pid = fork();

	switch(pid)
	{
		case 0: string = "I am children.\n";
		       k=3;
			break;
		case -1: perror("process creation failed\n");
		 	break;
		default: string = "I am parent";
			k=5;
			break;	 
	
	}
	while(k>0)
	{
		puts(string);
		sleep(1);
		k--;
	
	}


	exit(0);


}
