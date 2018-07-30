#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<grp.h>
#include<pwd.h>
#include"List.h"
int main(void)
{
	//初始化文件，目录，参数信息
	char filename[10][100],pram[20];
	filename[0][0] = '0';
	int n;
	for(n = 0;n < 20;n++)
		pram[n] = '0';

	int i,j,k,m;	//循环变量

	int count = 0;
	for(i = 1,j = 0,k = 0,m = 1;i < argc;i++)	{	//解析命令行参数
		if(argv[i][0] == '-') {
			while(argv[i][m])
				pram[j++] = argv[i][m++];
			m = 0;
		}
		else {
			strcpy(filename[k++],argv[i]);
			count++;
		}
	}
	
	
