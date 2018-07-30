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

void display_L(char *name,int count);												//实现单个文件的ls -l 功能
void display_name(char *path);											//实现ls -文件名/路径名 功能
void display_R(char *name,int count);									//实现ls -R功能
void display_RL(char *name,int count);									//实现ls -RL功能
void Sort(char ptr[][100],int length,int *a,char c);							//按照字母排序
void HpramHNfile(char ptr[][100],int length,char *pram);					//有参数有无文件
void NpramNfile(void);													//无参数无文件
void NpramHfile(char ptr[][100],int count);								//无参数有文件

int main(int argc,char *argv[])
{
	//初始化文件，目录，参数信息
	char filename[100][100],pram[20];
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
	if(argc == 1) {
		NpramNfile();
//		putchar('\n');
	}
	else if(pram[0] == '0') {
		NpramHfile(filename,count);
		putchar('\n');
	}
	else {
		HpramHNfile(filename,count,pram);
		putchar('\n');
	}
	return 0;
}

//无目录和文件无参数
void NpramNfile(void)
{
	DIR *dir;
	struct dirent *ptr;
	char file[100][100];
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
	
	Sort(file,count,ar,'N');
	int l;
	for(i = 0,l = 0;i < count;i++) {
		if(file[ar[i]][0] != '.') {
			struct stat buf;
			if(lstat(file[ar[i]],&buf) == -1) {
				printf("error");
				return;
			}
			l = l + strlen(file[ar[i]]);
			if(l%158 == 0)
				putchar('\n');
			if(S_ISDIR(buf.st_mode))
				printf("\033[34m %-10s \33[1m",file[ar[i]]);
			else if(buf.st_mode & 0111)
				printf("\033[32m %-10s \33[1m",file[ar[i]]);
			else
				printf("\033[37m %-10s \33[1m",file[ar[i]]);
		}
	}
	putchar('\n');
	closedir(dir);
	return;
}


//有参数，有无文件的ls命令处理函数
void HpramHNfile(char ptr[][100],int length,char *pram)
{
	char pt[1100][100];
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
					flag |= 00001;
				if(pram[j] == 'R')
					flag |= 00010;
				if(pram[j] == 'a')
					flag |= 00100;
				if(pram[j] == 'r')
					flag |= 01000;
		}
		
		if(count) {
			switch(flag) {
				case 00100:					//ls -a + 目录
					{
					if((dir = opendir(ptr[i])) == NULL) {
						return;
					}
					k = 0;
					while((str = readdir(dir)) != NULL) 
						strcpy(pt[k++],str->d_name);
						
					int b[k];
					for(j = 0;j < k;j++)
						b[j] = j;
					Sort(pt,k,b,'N');					//进行排序
					chdir(ptr[i]);
					for(j = 0;j < k;j++) {
						struct stat buf;
						if(lstat(pt[b[j]],&buf) == -1) {
							return;
						}
						if(S_ISDIR(buf.st_mode))
							printf("\033[34m %-10s \33[1m",pt[b[j]]);
						else if(buf.st_mode & 0111)
							printf("\033[32m %-10s \33[1m",pt[b[j]]);
						else	
							printf("\033[37m %-10s \33[1m",pt[b[j]]);
						}
						putchar('\n');
					
					closedir(dir);
					break;
					}			
				case 00001:					//ls -l + 目录
					{
					if((dir = opendir(ptr[i])) == NULL) {
						return;
					}
					chdir(ptr[i]);
					
					k = 0;
					while((str = readdir(dir)) != NULL)		//准备排序
						strcpy(pt[k++],str->d_name);
	
						int a[k];						//进行排序
						for(j = 0;j < k;j++)
							a[j] = j;
						Sort(pt,k,a,'N');

					for(j = 0;j < k;j++) {				//输出最后排序完的结果
						if(pt[a[j]][0] != '.') {
							display_L(pt[a[j]],0);
							putchar('\n');
						}
					}

					
					closedir(dir);
					break;
					}
				case 00101:					//ls -la + 目录
					{
					if((dir = opendir(ptr[i])) == NULL) {
						return;
					}
					chdir(ptr[i]);
					
					k = 0;
					while((str = readdir(dir)) != NULL)		//准备排序
						strcpy(pt[k++],str->d_name);
	
						int ar[k];						//进行排序
						for(j = 0;j < k;j++)
							ar[j] = j;
						Sort(pt,k,ar,'N');

					for(j = 0;j < k;j++) {			//输出最后排序完的结果
							
							display_L(pt[ar[j]],1);
							putchar('\n');
						}
					}
					break;
				case 00010:

					display_R(ptr[i],0);
					break;
				case 00110:
					display_R(ptr[i],1);
					break;
				case 00011:
					display_RL(ptr[i],0);
					break;
				case 00111:
					display_RL(ptr[i],1);
					break;
			}		
		}
		else {
			switch(flag) {
				case 00100:					//ls -a + 文件
				case 00101:
				case 00001:
					if(buf.st_mode & 0111)
						printf("\033[34m %s \33[1m",ptr[i]);
					else
						printf("\033[37m %s \33[1m",ptr[i]);
					break;
					putchar('\n');
				case 00010:
				case 00111:					//ls -l + 文件
				case 00011:
				case 00110:
					display_L(ptr[i],1);
					putchar('\n');
					break;
			}
		}
	}
}
			
					

void NpramHfile(char ptr[][100],int count)				//无参数有文件
{
	int i;
	for(i = 0;i < count;i++)
		display_name(ptr[i]);
}

//ls-文件名/路径
void display_name(char *path)
{
	int l = 0;
	int i,j,count = 0;
	struct stat buf;
	if(lstat(path,&buf) == -1) {
		perror(":");
		return;
	}
	if(S_ISDIR(buf.st_mode))
		count = 1;
	if(count) {
		DIR *dir;
		struct dirent *ptr;
		char filename[1000][100];
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

		Sort(filename,i,ar,'N');
		chdir(path);
		for(j = 0;j < i;j++) {
			if(filename[ar[j]][0] != '.') {
				l += strlen(filename[ar[j]]);
				if(l%158 == 0)
					putchar('\n');
				struct stat buf;
				if(lstat(filename[ar[j]],&buf) == -1) 
					return;
				if(S_ISDIR(buf.st_mode))	
					printf("\033[34m %-10s \33[1m",filename[ar[j]]);
				else if(buf.st_mode & 0111)
					printf("\033[32m %-10s \33[1m",filename[ar[j]]);
				else
					printf("\033[37m %-10s \33[1m",filename[ar[j]]);
			}
		}
		putchar('\n');
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
		printf("\033[37m %-10s \33[1m",name);
	}
	return;
}
	

//ls -l
void display_L(char *name,int count)
{
	char file_time[50];
	struct stat buf;
	if(lstat(name,&buf) == -1) {
		printf("获取文件%s数据失败",name);
		return;
	}
	if(name[0] != '.' || count) {
			if(S_ISLNK(buf.st_mode))
					printf("\033[37mi\33[1m");
			else if(S_ISREG(buf.st_mode))
					printf("\033[37m-\33[1m");
			else if(S_ISDIR(buf.st_mode))
					printf("\033[37md\33[1m");
			else if(S_ISCHR(buf.st_mode))
					printf("\033[37mc\33[1m");
			else if(S_ISBLK(buf.st_mode))
					printf("\033[37mb\33[1m");
			else if(S_ISFIFO(buf.st_mode))
					printf("\033[37mf\33[1m");
			else if(S_ISSOCK(buf.st_mode))
					printf("\033[37ms\33[1m");

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
	
		printf("\033[37m%5ld \033[1m",buf.st_size);					//文件大小
		
		strcpy(file_time,ctime(&buf.st_mtime));
		file_time[strlen(file_time) - 1] = '\0';
		printf("\033[37m%s \33[1m",file_time);				//最后一次修改的时间
	
		if(S_ISDIR(buf.st_mode))
				printf("\033[34m%s \33[1m",name);
		else if(buf.st_mode & 0111)
				printf("\033[32m%s \33[1m",name);
		else
				printf("\033[37m%s \33[1m",name);
	}		
}	

//ls -R 路径名
void display_R(char *name,int count)
{
	struct stat buf;
	struct dirent *ptr;
	DIR *dir;
	if(lstat(name,&buf) == -1) {
		perror("");
		return;
	}

	if(S_ISDIR(buf.st_mode)) {
		
		if((dir = opendir(name)) == NULL) {
			perror("");
			return;
		}
		printf("\033[34m %s\n\33[1m",name);
		
		while((ptr = readdir(dir)) != NULL) {
			if(ptr->d_name[0] != '.' || count)
				printf("\033[37m %s \33[1m",ptr->d_name);
		}
		closedir(dir);

		putchar('\n');
		if((dir = opendir(name)) == NULL) {
			perror("");
			return;
		}
		chdir(name);

		while((ptr = readdir(dir)) != NULL) 
			if(ptr->d_name[0] != '.')
				display_R(ptr->d_name,count);
		
		chdir("..");
		closedir(dir);
	}
}

void display_RL(char *name,int count)					//实现ls -RL功能
{
	struct stat buf;
	struct dirent *ptr;
	DIR *dir;
	if(lstat(name,&buf) == -1) {
		perror("");
		return;
	}

	if(S_ISDIR(buf.st_mode)) {
		if((dir = opendir(name)) == NULL) {
			perror("");
			return;
		}
		printf("\033[34m %-s\n\33[1m",name);
		chdir(name);	
		putchar('\n');
		while((ptr = readdir(dir)) != NULL) {		//输出目录下的所有文件，包括目录
			if(ptr->d_name[0] != '.' || count) {
				display_L(ptr->d_name,count);	
				putchar('\n');
			}
		}
		chdir("..");
		closedir(dir);
		putchar('\n');
		if((dir = opendir(name)) == NULL) {			//重新打开此目录，使文件描述符从开头开始
			printf("open the %s failed:",name);
			perror("");
			
			return;
		}
		chdir(name);	
		while((ptr = readdir(dir)) != NULL)			//读取目录下的文件，若为目录，则递归输出目录下的文件
			if(ptr->d_name[0] != '.')
				display_RL(ptr->d_name,count);
		chdir("..");
		closedir(dir);
	}
}
//对字符串进行排序,结果以游标形式保存在a数组中
void Sort(char ptr[][100],int length,int *a,char c)
{
	switch(c) {
		case 'N': {
			int i,j;//循环变量
			int swep;
			char str[2000][100];
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
			char str[10000][100];
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

