#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
int main(int argc, char **argv)
{
	int mode;//权限
	int mode_u;
	int mode_g;
	int mode_o;
	char *path;

	//检查参数个数的合理性
	if(argc < 3)
	{
		printf("%s <mode num> <target file>");
		exit(0);
	}

	//获取命令行参数
	mode = atoi(argv[1]);
	if(mode>777 && mode<0)
	{
		printf("wrong mode");
		exit(1);
	}
	
	//mode转换为8进制
	mode_u = mode / 100;
	mode_g = (mode - mode_u*100)/10;
	mode_o = mode - mode_u*100 - mode*10;
	mode = mode_u*8*8 + mode_g*8 + mode_o;

	//调用chmod()函数
	path = argv[2];
	if(chmod(path,mode)==-1)
	{
		perror("chmod error");
		exit(1);
	}

	return 0;
}
