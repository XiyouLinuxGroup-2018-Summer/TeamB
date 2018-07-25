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
void display_L(char *name);												//实现单个文件的ls -l 功能
void display_name(char *path);											//实现ls -文件名/路径名 功能
void display_R(char *name,int count);									//实现ls -R功能
void display_RL(char *name,int count);									//实现ls -RL功能
void Sort(char ptr[][50],int length,int *a);							//按照字母排序
void HpramHNfile(char ptr[][50],int length,char *pram);					//有参数有无文件
void NpramNfile(void);													//无参数无文件
void NpramHfile(char ptr[][50],int count);								//无参数有文件

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
	else if(pram[0] == '0')
		NpramHfile(filename,count);
	else
		HpramHNfile(filename,count,pram);
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


//有参数，有无文件的ls命令处理函数
void HpramHNfile(char ptr[][50],int length,char *pram)
{
	//处理无文件
	if(length == 0) {
		strcpy(ptr[0],".");
		length = 1;
	}
	int i,j,k;								//循环变量
	for(i = 0;i < length;i++) {
		int count = 0;
		int flag = 0;						//命令行参数转化为数字
		DIR *dir;
		struct dirent *str;
		struct stat buf;
		if(lstat(ptr[i],&buf) == -1) {
			printf("error");
			return;
		}

		if(S_ISDIR(buf.st_mode))			//判断是否为目录
			count = 1;
		
		for(j = 0;j < strlen(pram);j++) { //获取命令行参数
				if(pram[j] == 'l')
					flag |= 0001;
				if(pram[j] == 'R')
					flag |= 0010;
				if(pram[j] == 'a')
					flag |= 0100;
		}
		
		if(count) {
			switch(flag) {
				case 0100:					//ls -a + 目录
					if((dir = opendir(ptr[i])) == NULL) {
						return;
					}
					while((str = readdir(dir)) != NULL)
							printf(" %s ",str->d_name);
					closedir(dir);
					break;
								
				case 0001:					//ls -l + 目录
					if((dir = opendir(ptr[i])) == NULL) {
						return;
					}
					chdir(ptr[i]);
					while((str = readdir(dir)) != NULL)
						if(str->d_name[0] != '.') {
							display_L(str->d_name);
							putchar('\n');
						}
					
					closedir(dir);
					break;
				case 0101:					//ls -la + 目录
					if((dir = opendir(ptr[i])) == NULL) {
						return;
					}
					chdir(ptr[i]);
					while((str = readdir(dir)) != NULL) {
							display_L(str->d_name);
							putchar('\n');
					}
					
					closedir(dir);
					break;
				case 0010:
					display_R(ptr[i],0);
					break;
				case 0110:
					display_R(ptr[i],1);
					break;
				case 0011:
					display_RL(ptr[i],0);
					break;
				case 0111:
					display_RL(ptr[i],1);
					break;
			}		
		}
		else {
			switch(flag) {
				case 0100:					//ls -a + 文件
				case 0101:
				case 0001:
					printf(" %s ",ptr[i]);
					break;
				case 0010:
				case 0111:					//ls -l + 文件
				case 0011:
				case 0110:
					display_L(ptr[i]);
					putchar('\n');
					break;
			}
		}
	}
}
			
					

void NpramHfile(char ptr[][50],int count)				//无参数有文件
{
	int i;
	for(i = 0;i < count;i++)
		display_name(ptr[i]);
}

//ls-文件名/路径
void display_name(char *path)
{
	int i,j,count = 0;
	if(path[strlen(path) - 1] == '/')
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
		char name[20];
		for(i = 0,j = 0;i < strlen(path);i++) {
			if(path[i] == '/') {
				j = 0;
				continue;
			}
			name[j++] = path[i];
		}
		printf("%s",name);
	}
	return;
}
	

//ls -l
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

//ls -R 路径名
void display_R(char *name,int count)
{
	struct stat buf;
	if(lstat(name,&buf) == -1) {				//根据文件名获取信息
		printf("open the %s is error",name);
		return;
	}
	
	if(S_ISDIR(buf.st_mode)) {					//判断是否为目录
		DIR *dir;
		struct dirent *ptr;
		if((dir = opendir(name)) == NULL) {
			printf("open the %s is failed",name);//打开目录
			return;
		}
		chdir(name);								//进入下一级目录
		while((ptr = readdir(dir)) != NULL)			//获取下一级信息
			if(ptr->d_name[0] != '.')
				display_R(ptr->d_name,count);			//根据下一级文件名递归遍历
		chdir("..");							//遍历完下一级，返回上一级
		closedir(dir);					
	}
	else
		if(name[0] != '.' || count)
			printf(" %s ",name);
}

void display_RL(char *name,int count)					//实现ls -RL功能
{
	struct stat buf;
	if(lstat(name,&buf) == -1) {				//根据文件名获取信息
		printf("open the %s is error",name);
		return;
	}
	
	if(S_ISDIR(buf.st_mode)) {					//判断是否为目录
		DIR *dir;
		struct dirent *ptr;
		if((dir = opendir(name)) == NULL) {
			printf("open the %s is failed",name);//打开目录
			return;
		}
		chdir(name);								//进入下一级目录
		while((ptr = readdir(dir)) != NULL)			//获取下一级信息
			if(ptr->d_name[0] != '.')
				display_RL(ptr->d_name,count);			//根据下一级文件名递归遍历
		chdir("..");							//遍历完下一级，返回上一级
		closedir(dir);					
	}
	else
		if(name[0] != '.' || count) {
			display_L(name);
			putchar('\n');
		}
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

