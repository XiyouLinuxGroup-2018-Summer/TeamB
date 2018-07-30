#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<grp.h>
#include<pwd.h>
#include<time.h>
#include<errno.h>
#include"List.h"
typedef struct name{
	char names[100];
	struct name *next;
	struct name *prev;
}name_node_t,*name_list_t;

int g_len = 80;			//一行剩余长度
int g_maxlen = 0;			//最长文件名的长度
const char *FILENAME = "par.txt";
void display(char *name);
void Sort(char ptr[][100],int length,int *a,char c);

void display_R(char *name)
{
	struct stat buf;
	struct dirent *ptr;
	DIR *dir;

		if((dir = opendir(name)) == NULL) {
			perror(":");
			return;
		}
		chdir(name);
		while((ptr = readdir(dir)) != NULL) {
			lstat(ptr->d_name,&buf);
			if(S_ISDIR(buf.st_mode)) {
				if(strcmp(".",ptr->d_name) == 0 || strcmp("..",ptr->d_name) == 0)
				continue;
				printf("\033[34m %-s\n\33[1m",ptr->d_name);
				display_R(ptr->d_name);
			}
			else {

				printf("\033[37m %-s \33[1m",ptr->d_name);
			}
			putchar('\n');

		}
		chdir("..");
		closedir(dir);
	
}
/*
{	

	static long l = 0;
	struct stat buf;
	if(lstat(name,&buf) == -1) {				//根据文件名获取信息
		perror(":");	
		return;
	}
	
	if(S_ISDIR(buf.st_mode)) {					//判断是否为目录
	
		DIR *dir;
		struct dirent *ptr;
		if((dir = opendir(name)) == NULL) {
			perror(":");
			return;
		}
	//	putchar('\n');
		chdir(name);								//进入下一级目录
		char pt[100][100];
		int cnt = 0;
		int i;
		while((ptr = readdir(dir)) != NULL)			//获取下一级信息
			strcpy(pt[cnt++],ptr->d_name);
			
			int ar[cnt];
			for(i = 0;i < cnt;i++)
				ar[i] = i;
			Sort(pt,cnt,ar,'N');					//对目录和文件进行整合排序
		

		for(i = cnt-1;i > 0;i--)
			if(pt[ar[i]][0] != '.') {
				l++;
				display_R(pt[ar[i]],(head));			//根据下一级文件名递归遍历
			}
		name_node_t *newNode1 = (name_node_t *)malloc(sizeof(name_node_t));
		strcpy(newNode1->names,"\n");
		List_InsertAfter((*head),newNode1);
		for(i = cnt-1;i > 0;i--) {
			name_node_t *newNode = (name_node_t *)malloc(sizeof(name_node_t));
			strcpy(newNode->names,pt[ar[i]]);
			List_InsertAfter((*head),newNode)
		}
		name_node_t *newNode2 = (name_node_t *)malloc(sizeof(name_node_t));
		char str[100];
		getcwd(str,100);
		//strcpy(str,name);
		strcat(str,"\n");
		strcpy(newNode2->names,str);
		List_InsertAfter((*head),newNode2);
	
		
		chdir("..");								//遍历完下一级，返回上一级
		closedir(dir);
	}
	
}
*/	
/*	
	DIR *dir;
	struct dirent *ptr;
	struct stat *buf;
	if((dir = opendir(name)) == NULL && (lstat(name,buf)) == -1) {
		printf("error");
		return;
	}
	if(dir) {
		chdir(name);
		while((ptr = readdir(dir)) != NULL) {
			if(ptr->d_name[0] != '.')
				display_R(ptr->d_name);
		}
		closedir(dir);
	}
	else {
		printf(" %s ",name);
		
	}
	return;
*/
/*	struct stat buf;
	
	if(lstat(name,&buf) == -1) {			//根据文件名获取信息
		printf(" error ");
		return;
	}

	if(S_ISDIR(buf.st_mode)) {				//判断是否为目录，若为文件则直接输出
		struct dirent *ptr;
		DIR *dir;

		if((dir = opendir(name)) == NULL) {	//打开目录
			printf("opendir is failed");
			return;
		}
		chdir(name);						//进入下一级目录

		while((ptr = readdir(dir) ) != NULL)//获取下一级信息
			if(ptr->d_name[0] != '.')
				display_R(ptr->d_name);			//根据下一级文件名进入display_R中判断

		chdir("..");							//遍历完下一级目录，返回上一级	
		closedir(dir);
		
	}
	else
		printf(" %s ",name);
	
		
	return;*/


int main(void)
{
	char *path = ".";
	display_R(path);
	
	return 0;	
}
void display(char *name)
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
	

	printf(" %ld ",buf.st_nlink);

	struct passwd *psd = getpwuid(buf.st_uid);	//文件所有者用户名
	struct group *grp = getgrgid(buf.st_gid);	//文件用户组组名
	printf("%s ",psd->pw_name);
	printf("%s ",grp->gr_name);

	printf("%ld ",buf.st_size);					//文件大小

	strcpy(file_time,ctime(&buf.st_mtime));
	file_time[strlen(file_time) - 1] = '\0';
	printf("%s ",file_time);				//最后一次修改的时间

	printf("%s ",name);
	
}
//对字符串进行排序,结果以游标形式保存在a数组中
void Sort(char ptr[][100],int length,int *a,char c)
{
	switch(c) {
		case 'N': {
			int i,j;//循环变量
			int swep;
			char str[100][100];
			for(i = 0;i < length;i++)
				strcpy(str[i],ptr[i]);
				
			for(i = 0;i < length;i++)						//统一大小写字母
				if(str[i][0] >= 'A' && str[i][0] <= 'Z')
					str[i][0] += 32;
		
			for(i = 0;i < length;i++)
				for(j = i + 1;j < length;j++)
					if(strcmp(str[a[i]],str[a[j]]) > 0) {
						swep = a[i];
						a[i] = a[j];
						a[j] = swep;
					}
		}
			break;
		case 'R': {
			int i;
			int j;
			int count = 0;
			struct stat buf;
			int swep;
			char str[100][100];
			for(i = 0;i < length;i++) {						//把所有目录整合到一起
				if(lstat(ptr[i],&buf) == -1)
					return;
				if(S_ISDIR(buf.st_mode)) {
					swep = a[i];
					a[i] = count;
					a[count++] = swep;
				}
			}
		
			for(j = 0;j < count;j++)						//对目录进行排序
				strcpy(str[i],ptr[i]);
			for(j = 0;j < count;j++)						//统一大小写字母
				if(str[j][0] >= 'A' && str[j][0] <= 'Z')
					str[j][0] += 32;
		
			for(i = 0;i < count;i++) {
				for(j = i + 1;j < length;j++)
					if(strcmp(str[a[i]],str[a[j]]) > 0) {
						swep = a[i];
						a[i] = a[j];
						a[j] = swep;
					}
			}


			for(i = count;i < length;i++)					//对文件进行排序
				strcpy(str[i],ptr[i]);
				
			for(i = count;i < length;i++)						//统一大小写字母
				if(str[i][0] >= 'A' && str[i][0] <= 'Z')
					str[i][0] += 32;
		
			for(i = count;i < length;i++)
				for(j = i + 1;j < length;j++)
					if(strcmp(str[a[i]],str[a[j]]) > 0) {
						swep = a[i];
						a[i] = a[j];
						a[j] = swep;
					}
			}
			break;
		case 'r': {											//反向排序	
			int i,j;
			int swep;
			for(i = 0;i < length;i++)
				if(ptr[i][0] >= 'A' && ptr[i][0] <= 'Z')
					ptr[i][0] += 32;
			for(i = 0;i < length;i++)
				for(j = i + 1;j < length;j++)
					if(strcmp(ptr[a[i]],ptr[a[j]]) < 0 )  {
						swep = a[i];
						a[i] = a[j];
						a[j] = swep;
					}
			}
			break;
		
	}		
	
}

