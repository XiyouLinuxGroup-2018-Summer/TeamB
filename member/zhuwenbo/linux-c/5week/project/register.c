#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>

#ifndef _MY_ERR_H
#define _MY_ERR_H
#include "my_err.h"
#endif

int main(int argc, char *argv[])
{
	if(argc < 3)
		my_err("input error", __LINE__);
	int fd;
	if( (fd = open("usr.txt", O_RDWR)) < 0)
		my_err("open", __LINE__);
	char name[20];
	strcpy(name, argv[1]);                           //复制用户名
	if(write(fd, name, strlen(name) + 1) < 0)
		my_err("write", __LINE__);
	char passwd[20];
	strcpy(passwd, argv[2]);						//复制密码
	if(write(fd, passwd, strlen(passwd) + 1) < 0)
		my_err("write", __LINE__);
}
