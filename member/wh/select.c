#include<stdio.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
int main(void)
{
	fd_set rd;					//读文件描述符集合
	struct timeval tv;			//时间间隔
	int err;					//错误值
								//监视标准输入是否可以读取数据
	FD_ZERO(&rd);				
	FD_SET(0,&rd);				
	//设置5秒的等待超时
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	err = select(1,&rd,NULL,NULL,&tv);

	if(-1 == err)
		perror("");
	else if (err)
		printf("data is available now.\n");
	else
		printf("No data within five seconds.\n");
	return 0;
}
