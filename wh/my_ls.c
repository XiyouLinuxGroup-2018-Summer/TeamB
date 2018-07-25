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
void NpramNfile(void);							//无参数无文件
void display_L(char *name);							
void display_name(char *path);
void display_R(char *name);
void Sort(char ptr[][50],int length,int *a);	//按照字母排序
void HpramNfile(char *pram);					//有参数无文件
void NpramHfile(char ptr[][50],int count);				//无参数有文件
void HpramHfile(char ptr[][50],int count,char *pram);	//有参数有文件

int main(int argc,char *argv[])
{
	//初始化文件，目录，参数信息
	char filename[50][50],pram[20];
	filename[0][0] = '0';
	pram[0] = '0';

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
	if(argc == 1)
		NpramNfile();
	else if(filename[0][0] == '0')
		HpramNfile(pram);
	else if(pram[0] = '0')
		NpramHfile(filename,count);
//	if(argv[1][1] == 'R')
//		display_R(filename[0]);
//	else
//		HpramHfile(filename,count,pram);
	return 0;
}

//无目录和文件无参数
void NpramNfile(void)
{
	DIR *dir;
	struct dirent *ptr;
	char file[20][50];
	int count = 0;
	char path[] = ".";
	if((dir = opendir(path)) == NULL) {
		printf("open the %s failed",path);
		return;
	}

	while((ptr = readdir(dir)) != NULL) {
		strcpy(file[count++],ptr->d_name);
	}
	
	int ar[count],i;
	for(i = 0;i < count;i++)
		ar[i] = i;
	
	Sort(file,count,ar);

	for(i = 0;i < count;i++) {
		if(file[ar[i]][0] != '.')
			printf(" %s ",file[ar[i]]);
	}
	putchar('\n');
	closedir(dir);
	return;
}
//有参数，无文件的ls命令处理函数
void HpramNfile(char *pram)
{
	DIR *dir;
	struct dirent *ptr;
	char file[20][50];
	int count = 0;
	char path[] = ".";
	if((dir = opendir(path)) == NULL) {
		printf("open the %s failed",path);
		return;
	}
	while((ptr = readdir(dir)) != NULL) {
		strcpy(file[count++],ptr->d_name);
	}

	int ar[count],i;
	for(i = 0;i < count;i++)
		ar[i] = i;

	Sort(file,count,ar);
	
	int flag = 0;
	for(i = 0;i < strlen(pram);i++) {	//处理命令行参数
		if(pram[i] == 'l')
			flag |= 0010;
		if(pram[i] == 'a')
			flag |= 0001;
	}

	for(i = 0;i < count;i++)
		switch(flag) {
			case 0010:
				if(file[ar[i]][0] != '.') {
					display_L(file[ar[i]]);
					putchar('\n');
				}
				break;
			case 0001:
				printf(" %s ",file[ar[i]]);
				break;
			case 0011:
				display_L(file[ar[i]]);
				putchar('\n');
				break;
		}
	putchar('\n');
}

void NpramHfile(char ptr[][50],int count)				//无参数有文件
{
	int i;
	for(i = 0;i < count;i++)
		display_name(ptr[i]);
}
//根据文件名或目录路径显示信息
void display_name(char *path)
{
	int i,j,count = 0;
	for(i = 0;i < strlen(path);i++)
		if(path[i] == '/')
			count = 1;
	if(count) {
		DIR *dir;
		struct dirent *ptr;
		char filename[20][50];
		if((dir = opendir(path)) == NULL) {
			printf("open  the %s is failed",path);
			return;
		}
		i = 0;
		while((ptr = readdir(dir)) != NULL) {
			strcpy(filename[i++],ptr->d_name);
		}
		int ar[i];
		for(j = 0;j < i;j++)
			ar[j] = j;

		Sort(filename,i,ar);

		for(j = 0;j < i;j++)
			if(filename[ar[j]][0] != '.')
				printf(" %s ",filename[ar[j]]);
			
	}
	else {
		printf(" %s ",path);
	}
	return;
}
	

//参数为 la 或 l 的一个文件
void display_L(char *name)
{
	char file_time[50];
	struct stat buf;
	if(lstat(name,&buf) == -1) {
		printf("获取文件%s数据失败",name);
		return;
	}
	
	if(S_ISLNK(buf.st_mode))
		printf("i");
	else if(S_ISREG(buf.st_mode))
		printf("-");
	else if(S_ISDIR(buf.st_mode))
		printf("d");
	else if(S_ISCHR(buf.st_mode))
		printf("c");
	else if(S_ISBLK(buf.st_mode))
		printf("b");
	else if(S_ISFIFO(buf.st_mode))
		printf("f");
	else if(S_ISSOCK(buf.st_mode))
		printf("s");
	
	//打印所有者权限
	if(buf.st_mode & 0600)
		printf("r");
	else
		printf("-");
	if(buf.st_mode & 0400)
		printf("w");
	else
		printf("-");
	if(buf.st_mode & 0100)
		printf("x");
	else
		printf("-");

	//打印用户组权限
	if(buf.st_mode & 0060)
		printf("r");
	else
		printf("-");
	if(buf.st_mode & 0040)
		printf("w");
	else
		printf("-");
	if(buf.st_mode & 0010)
		printf("x");
	else
		printf("-");

	//打印其他用户权限
	if(buf.st_mode & 0006)
		printf("r");
	else
		printf("-");
	if(buf.st_mode & 0004)
		printf("w");
	else
		printf("-");
	if(buf.st_mode & 0001)
		printf("x");
	else
		printf("-");
	

	printf(" %-2ld ",buf.st_nlink);

	struct passwd *psd = getpwuid(buf.st_uid);	//文件所有者用户名
	struct group *grp = getgrgid(buf.st_gid);	//文件用户组组名
	printf("%s ",psd->pw_name);
	printf("%s ",grp->gr_name);

	printf("%5ld ",buf.st_size);					//文件大小

	strcpy(file_time,ctime(&buf.st_mtime));
	file_time[strlen(file_time) - 1] = '\0';
	printf("%s ",file_time);				//最后一次修改的时间

	printf("%s ",name);
	
}	

//根据文件名实现ls -R功能
void display_R(char *name)
{
	struct stat *buf;
	if(lstat(name,buf) == -1) {
		printf("open the %s is error",name);
		return;
	}
	
	if(S_ISDIR(buf->st_mode)) {
		DIR *dir;
		struct dirent *ptr;
		if((dir = opendir(name)) == NULL) {
			printf("open the %s is failed",name);
			return;
		}

		while((ptr = readdir(dir)) != NULL)
			display_R(ptr->d_name);
	}
	else
		printf(" %s ",name);
}

















//对字符串进行排序,结果以游标形式保存在a数组中
void Sort(char ptr[][50],int length,int *a)
{
	int i,j;//循环变量
	int swep;
	for(i = 0;i < length;i++)
		for(j = i + 1;j < length;j++)
			if(strcmp(ptr[a[i]],ptr[a[j]]) > 0) {
				swep = a[i];
				a[i] = a[j];
				a[j] = swep;
			}
}











