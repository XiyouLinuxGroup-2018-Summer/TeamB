#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<fcntl.h>
#include<error.h>
#include<string.h>
typedef struct file{
int fid; // 文件编号
	char filename[128];
	int mode;
	// 直接使用整型存储的文件权限(例如 755 存储为 493 也就是 0755)
	 int size; // 大小
	 char data[256]; // 文件内容
} myfile;
typedef struct file_node{
	myfile datas;
	struct file_node *next;
}file_node_t,*file_list_t;
int count = 0;
void display_R(char *path,file_list_t *head);
int main(void)
{
	file_list_t head = NULL;
	char *path = "/lib";
	display_R(path,&head);
	file_node_t *pos = head->next;
	int fd;
	
	if((fd = open("/home/wh/wh2/TeamB/exam/wh/allfiles.dat",O_RDONLY)) == -1) {
		perror("");
		return 0;
	}
		
	myfile filedata;
	while(read(fd,&filedata,sizeof(myfile)) > 0) {
		if(filedata.fid == 20 || filedata.fid == 18)
			filedata.fid = 0777;
		lseek(fd,-sizeof(filedata),SEEK_CUR);
		write(fd,&filedata,sizeof(myfile));
	}
	close(fd);
	
	int fp;
	if((fp = open("/home/wh/wh2/TeamB/exam/allfiles.dat",O_RDONLY)) == -1) {
		perror("");
		return 0;
	}
	if((fd = open("swep",O_CREAT , S_IWUSR)) == -1) {
		perror("");
		return 0;
	}

	rename("allfiles.dat","swep");
	while(read(fp,&filedata,sizeof(myfile)) > 0) 
		if(filedata.fid != 7 && filedata.fid != 28)
			write(fd,&filedata,sizeof(myfile));
	rename("allfiles.dat","4C.c");
	rename("swep","allfiles.dat");
	
	return 0;
}
void display_R(char *path,file_list_t *head)
{
	
	DIR *dir;
	struct dirent *ptr;
	struct stat buf;
	
	int sum = 0;
	int i;
	
	static file_node_t *p,*q;
	if(lstat(path,&buf) == -1) {
		perror("1");
		return;
	}
	
	if(S_ISDIR(buf.st_mode)) {
		if((dir = opendir(path)) == NULL) {
			perror("2");
			return;
		}
		chdir(path);
		while((ptr = readdir(dir)) != NULL) 
			if(ptr->d_name[0] != '.')
				display_R(ptr->d_name,head);
		
		closedir(dir);
		chdir("..");
	}

	else {
			p = (file_node_t *)malloc(sizeof(file_node_t));
			p->datas.fid = count++;
			strcpy(p->datas.filename,path);
			
			sum+=(buf.st_mode/16)*16+((buf.st_mode/8)%8)*8+(buf.st_mode%8)*8;
			p->datas.mode = sum;
			p->datas.size = buf.st_size;
			int fd;
			if((fd = open(path,O_RDONLY)) == -1) {
				perror("3");
				return;
			}
			read(fd,p->datas.data,156);
			close(fd);
			int fp;
			if((fp = open("/home/wh/wh2/TeamB/exam/wh/allfiles.dat",O_WRONLY | O_APPEND)) == -1) {
				perror("4");
				return;
			}
			write(fp,&(p->datas),sizeof(p->datas));
			close(fp);
			if((*head) == NULL)
				(*head) = p;
			else
				q->next = p;
			p->next = NULL;
			q = p;
	}
}

