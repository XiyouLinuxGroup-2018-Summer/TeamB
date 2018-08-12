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
	char w[100] = "hello";
	char r[100];
	int wfd, rfd;
	int len;
	umask(0);
	if(mkfifo("writefifo", S_IFIFO | 0666))
	{
		printf("create fifo is failed because %s\n", strerror(errno));
		exit(0);
	}
	umask(0);
	wfd = open("writefifo", O_WRONLY);
	while((rfd = open("readfifo", O_RDONLY)) == -1)
	{
		sleep(1);
	} 
	while(1)
	{
		write(wfd, w, strlen(w) + 1);
		len = read(rfd, r, 100);
		printf("read error %s\n", strerror(errno));
		printf("pxz: %s\n", r);
		printf("len = %d\n", len);
	}
	close(wfd);
	close(rfd);
	exit(0);
}
