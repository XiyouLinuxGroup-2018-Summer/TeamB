#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
int main(void)
{
	int fd;
	char buf[1024];
//	umask(0);
	if( (fd = open("myfifo", O_RDONLY | 0666)) == -1)
	{
		perror("open failed");
		exit(0);
	}
	read(fd, buf, 1024);
	printf("read content: %s\n", buf);
	close(fd);
	exit(0);
}
