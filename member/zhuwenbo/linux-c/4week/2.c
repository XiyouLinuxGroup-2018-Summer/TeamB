#include<stdio.h>        
#include<stdlib.h>       
#include<string.h>       
#include<unistd.h>       
#include<fcntl.h>        
#include<sys/types.h>    
#include<sys/stat.h>     
#include<errno.h>    

int main()
{
	char w[100] = "hi";
	w[2]= 0;
	char r[100];
	int wfd, rfd;
	umask(0);
	if(mkfifo("readfifo", S_IFIFO | 0666))
	{
		 printf("create fifo is failed because %s\n", strerror(errno));
         exit(0);  
	}
	umask(0);
	while((rfd = open("writefifo", O_RDONLY)) == -1)
	{
		sleep(1);
	}
	wfd = open("readfifo", O_WRONLY);
	while(1)
	{
		int len = read(rfd, r, 100);
		printf("len = %d\n", len);
		printf("zwb: %s\n", r);
		write(wfd, w, strlen(w) + 1);
	}
	close(wfd);
	close(rfd);
	exit(0);
}
