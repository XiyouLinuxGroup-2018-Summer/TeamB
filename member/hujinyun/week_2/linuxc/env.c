#include<stdio.h>
//#include<malloc.h>

//通过系统预定义的全局变量environ显示各个环境变量的值

//所以此处定义名字必须是environ+外部全局变量
extern char **environ;
int main(int argc, char **argv)
{
//	char **envir;
	int i;

	for(i=0; i<argc; i++)
	{
		printf("argv[%d] is:%s\n",i,argv[i]);
	
	}

	for(i=0; environ[i]!= NULL; i++)
	{
		printf("%s\n",environ[i]);
	
	}
	return 0;

}
