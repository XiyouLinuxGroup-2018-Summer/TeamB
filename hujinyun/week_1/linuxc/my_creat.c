#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>

int main()
{
	int fd;
	if((fd = open("example_1.c",O_CREAT | O_EXCL , S_IRUSR, S_IWUSR)) == -1) //可读可写 O_EXCL标志若文件不存在，创建。若存在打开错误
	//if((fd = creat("example_1.c",S_IRWXU)) == -1 )
	{
		perror("open");
		//printf("open :%s  with errno:%d\n",strerror((errno),errno);
		exit(1);
		
	}
	else
	{
		printf("creat fill success");
	}
	close(fd);
	return 0;
}
