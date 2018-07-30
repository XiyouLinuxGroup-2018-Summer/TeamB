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
#define PARAM_R 4 // -r按文件首字母逆序输出
#define MAXROWLEN 80 //一行显示的最多字符数

int g_leave_len = MAXROWLEN; //一行剩余长度，用于对齐
int g_maxlen = 0; //存放某目录下最长文件名的长度

/*错误输出函数*/
void my_err(char *err_string, int line)
{
	fprintf(stderr,"line: %d",line);
	perror("err_string");
	exit(1);
}

/*-l打印*/
int display_attribute(struct stat buf,char *name)//获取文件属性并打印
{
	char buf_time[32];
	struct passwd *psd; //获取文件所有者的用户名 头文件为 pwd.h
	struct group *grp; //获取文件所有者的组名

	if(S_ISREG(buf.st_mode))
	{
		printf("-");
	}
	else if(S_ISDIR(buf.st_mode))
	{
		printf("d");
	}

	else if(S_ISLNK(buf.st_mode))
	{
		printf("l");
	}

	else if(S_ISCHR(buf.st_mode))
	{
		printf("c");
	}

	else if(S_ISBLK(buf.st_mode))
	{
		printf("b");
	}
	else if(S_ISFIFO(buf.st_mode))
	{
		printf("f");
	}
	else if(S_ISSOCK(buf.st_mode))
	{
		printf("s");
	}
	/*获取并打印文件所有者权限*/
	if(buf.st_mode & S_IRUSR)
	{
		printf("r");
	}
	else
		printf("-");
	if(buf.st_mode & S_IWUSR)
	{
		printf("w");
	}
	else
		printf("-");
	if(buf.st_mode & S_IXUSR)
	{
		printf("x");
	}
	else
		printf("-");
		
	/*获取并打印文件用户组权限*/
	if(buf.st_mode & S_IRGRP)
	{
		printf("r");
	}
	else
		printf("-");
	if(buf.st_mode & S_IWGRP)
	{
		printf("w");
	}
	else
		printf("-");
	if(buf.st_mode & S_IXGRP)
	{
		printf("x");
	}
	else
		printf("-");

	if(buf.st_mode & S_IROTH)
	{
		printf("r");
	}
	else
		printf("-");
	if(buf.st_mode & S_IWOTH)
	{
		printf("w");
	}
	else
		printf("-");
	if(buf.st_mode & S_IXOTH)
	{
		printf("x");
	}
	else
		printf("-");
	
	printf("4%d ",buf.st_nlink);
	psd = getpwuid(buf.st_uid);
	grp = getgrgid(buf.st_gid);
	printf("%-8s",psd->pw_name);
	printf("%-8s",grp->gr_name);

	printf("%6d",buf.st_size);
	/*打印最后修改时间*/
	strcpy(buf_time, ctime(&buf.st_mtime)); //注意这里放&
	buf_time[strlen(buf_time)-1]='\0';  //将换行符去掉
	printf("  %s",buf_time);
	printf("  %-s\n",name);
 	
}


int display_len(char *name)
{
	int i,sum = 0;
	for(i=0;i<strlen(name); i++)
	{
		if(name[i]<0) //
			sum++;
	}
	sum/=3;
	return (strlen(name)-sum);
}
/*打印所有的文件名（只打印文件名）
 *此函数每次只打印一个
 *主要用于输出对齐
 * */
int display_single(char *name)//打印所有文件名（上下对齐）
{
	int i,len;
	
	//如果本行不足以打印一个文件名则换行
	len = display_len(name);
/*	if(len > g_leave_len)
	{
		printf("\n");
		g_leave_len = MAXROWLEN;
	}
*/	
	if(g_leave_len <g_maxlen)
	{
		printf("\n");
		printf("打印正确");
		g_leave_len = MAXROWLEN;//重新将leave_len赋为最长值
	}
	len = MAXROWLEN - len; //此时的len表示输出空格数
	printf("%-s",name);
/*	for(i=0;i<len; i++)
	{
		printf(" ");
	}
	printf("  ");
*/	g_leave_len -= (g_maxlen + 2);

}


int display(int flag,char *pathname)//根据命令行参数和完整路径名显示目标文件
{
	struct stat buf;
	int i,j;
	char name[NAME_MAX+1];

		pathname[strlen(pathname)]='\0';
	   if(lstat(pathname,&buf)== -1)
        my_err("lstat",__LINE__);
        

	switch(flag)
	{
		case PARAM_NONE:
							if(pathname[0]!='.')
							{
								display_single(pathname);
							}
							break;

		case PARAM_A:	display_single(pathname);
						break;

		case PARAM_L:   
						if(pathname[0]!='.')
						{
							display_attribute(buf,pathname);
						}
						break;

/*		case PARAM_R:	
						if(pathname[0]!='.')
						{
							display_recursive_1(pathname);
						}
						break;
	
		 
		case PARAM_A+PARAM_L:	display_attribute(pathname);
								break;
		
		case PARAM_A+PARAM_R:	display_recursive_1(pathname);
								break;
		
		case PARAM_L+PARAM_R:	if(pathname[0]!='.')
								{
									display_recursive_2(pathname);
								}							
								break;
*/
	}



}
void display_dir(int flag_param, char *path)//
{
	struct dirent *ptr;
	DIR *dir;
	int i,j;
	char filename[256][PATH_MAX+1] , temp[PATH_MAX+1];

	dir = opendir(path);
	int count=0;
	if(dir== NULL)
	{
		my_err("dir",__LINE__);
	}

	while((ptr = readdir(dir))!=NULL )
	{
		if(g_maxlen < strlen(ptr->d_name))
		{
			g_maxlen = strlen(ptr->d_name);
			strcpy(filename[i],ptr->d_name);
			filename[i][strlen(ptr->d_name)] = '\0';
			count++;
		}
	}
	
		//排序
		for(i=0; i<count;i++)
			for(j=0; j<count-1-i; j++)
			{
				if(strcmp(filename[j],filename[j+1])>0 )
				{
					strcpy(temp,filename[j+1]);
					temp[strlen(filename[j+1])] = '\0';
					strcpy(filename[j+1],filename[j]);
					filename[j+1][strlen(filename[j])] = '\0';
					strcpy(filename[j],temp);
					filename[j][strlen(temp)] = '\0';
				}
			}


		for(i=0; i<count; i++)
		{
			display(flag_param,filename[i]);
		} 

		closedir(dir);

		//没有-l打印回车
		if((flag_param & PARAM_L)==0)
		{
			printf("\n");
		}
		
}


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
			for(k=1;k<strlen(argv[i]); k++,j++)
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
		return 0;
	}
	
	i=1;
	do
	{
		//如果不是目标文件或目录，解析一下命令行参数
		
		//先找到对应路径
		if( argv[i][0] == '-')
		{
			i++;
			continue; //进行下一圈
		}

		else
			strcpy(path,argv[i]);
		
		//目标文件不存在，报错退出
		if(stat(path,&buf)== -1)
		{
			my_err("stat",__LINE__);
		}

		if(S_ISDIR(buf.st_mode))//首先判断是一个目录
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
