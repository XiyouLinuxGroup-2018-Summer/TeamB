#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>

#ifndef _MY_ERR_H
#define _MY_ERR_H
#include "my_err.h"
#endif
char user[10][21];
char passwd[10][21];
#define PORT 8848

void username(void)
{
	int fd;
	int count = 0;
	int i = 0;
	int flag = 0;
	char temp;
	if( (fd = open("user.txt", O_RDONLY)) == -1)
		my_err("open", __LINE__);
	while(read(fd, temp, 1))        //将文件中用户名和密码读取出
	{
		if(temp != '\0' && flag = 0)
		{
			name[count][i++] = temp;
			continue;
		}
		name[count][i] = '\0';
		i = 0;
		if(temp != '\0')
		{
			passwd[count][i++] = temp;
			continue;
		}
		count++;
		flag = i = 0;
	}
}
int login(char *str, char *key)
{
	for(int i = 0; i < 10; i++)
	{
		if( (strcmp(str, name[i]) == 0) && (strcmp(key, passwd[i]) == 0) )
			return 1;
	}
	return 0;
}
int main()
{
	int ser_fd, cli_fd;
	struct sockaddr_in ser_addr, cli_addr;
	socklen_t socklen;
	if( (ser_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		my_err("socket", __LINE__);
}
