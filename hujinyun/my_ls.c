#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<unistd.h>
#include<linux/limits.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<errno.h>

#define PARAM_NONE 0 //无参数
#define PARAM_A 1 // -a显示所有文件（包括隐藏文件）
#define PARAM_L 2 // -l显示文件详细信息
#define PARAM_R 3 // -r按文件首字母逆序输出
#define MAXROWLEN 80 //一行显示的最多字符数

int g_leave_len = MAXROWLEN; //一行剩余长度，用于对齐
int g_maxlen; //存放某目录下最长文件名的长度

int display_attribute(struct stat buf,char*name);

int main(int argc, char **argv)
{
	int i,j,k,num;
	char path[PATH_MAX+1];
	char param[32]; //用于保存命令行参数 -a等
	int flag_param = PARAM_NONE; //保存参数的种类

	struct stat buf;//保存文件状态的结构体、

	/*命令行参数的解析，分析 -l, -a, -R, -al, -la, -aR,-Ra,-lR,-Rl,-alR,-aRl,-laR,-lRa,-Ral,-Rla 选项*/
	j=0;
	num=0;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			for(k=1;k<strlen(argv[i]); k+,j++)
			{
				param[j]=argv[i][k]; //将argv的参数保存到param数组中
			}
		 	num++;//保存‘-’的个数
		}
	}
	//支持选项a,l，R
	for(i=0; i<j; i++)
	{
		if(param[i] == 'a')
		{
			flag_param |= PARAM_A;
			continue;
		}
		else if(param[i] == 'l')
		{
			flag_param |= PARAM_L;
			continue;
		}
		else if(param[i] == 'R')
		{
			flag_param |= PARAM_R;
			continue;
		}
		else
		{
			printf("my_ls:invalid option: %c\n",param[i]);
			exit(1);
		}

	}

	param[j]='\0';
	
	//若果没有输入文件名或目录，就显示当前文件名（此时是只输入 ls 没有参数也没有路径的情况）
	if(argc==(num + 1))
	{
		strcpy(path,"./");
		path[2]='\0';
		display_dir(flag_param,path);
		returb 0;
	}
	i=1;
	do
	{
		//如果不是目标文件或目录，解析一下命令行参数
		if( argv[i][0] = '-')
		{
			i++;
			continue; //进行下一圈
		}
		else
			strcpy(path,argv[i]);
		//目标文件不存在，报错退出
		if(stat(path,&buf)== -1)
		{
			my_err("stat")
		}

		if(S_SDIR(buf.st_mode))//首先判断是一个目录
		{
			if(path[strlen(argv[i]-1)]!='/')//如果目录的最后一个字符不是‘/’，就加上'/'
			{
				path[strlen(argv[i])]='/';
				path[strlen(argv[i]+1)]='\0';
			
			}
			else
			{
				path[strlen(argv[i])]='\0';
			
			}
			display_dir(flag_param,path);
			i++;
		}
		else //argv[i]是一个文件
		{	display(flag_param,path);
			i++;
		}

	
	}while(i<argc);
	return 0;
	
}
