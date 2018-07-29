#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<time.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<grp.h>
#include<pwd.h>
#include<errno.h>
#include<fcntl.h>

typedef struct file
{
	int fid; 					// 文件编号
 	char filename[256];
	int mode;					// 直接使用整型存储的文件权限(例如 755 存储为 493 也就是 0755)
    int size; 					// 大小
    char data[256]; 			// 文件内容
	struct file *next;
}myfile;

int count = 0;

void print(char *name, myfile *files)
{       
		int fd;
        int mode;
		int mode_u = 0, mode_g = 0, mode_o = 0;
        char buf_time[32];
		struct stat buf;
		strcpy(files->filename, name);
		fd = open(name, O_RDONLY, S_IRUSR|S_IWUSR);
		read(fd, files->data, 255);
        lstat(files->filename, &buf);
        mode = buf.st_mode;
        
        /*获取并打印文件所有者的权限*/
        if(mode & S_IRUSR)
                mode_u += 4;
        if(mode & S_IWUSR)
               	mode_u += 2;
        if(mode & S_IXUSR)
                mode_u += 1;
         /*获取并打印文件所属组的权限*/
        if(mode & S_IRGRP)
                mode_g += 4;
        if(mode & S_IWGRP)
                mode_g += 2;
        if(mode & S_IXGRP)
                mode_g += 1;
        
        /*获取并打印文件其他用户的权限*/
        if(mode & S_IROTH)                                                                                                                              
				mode_o += 4;
        if(mode & S_IWOTH)
                mode_o += 2;
        if(mode & S_IXOTH)
              	mode_o += 1;
		files->mode = mode_u * 10 * 10 + mode_g * 10 + mode_o;
         files->size  = buf.st_size;
		 close(fd);
}

void display(char *path, myfile **head)
{
	//printf("path :%s\n", path);
	struct stat buf;
	DIR *dir;
	struct dirent *ptr;
	char pathname[1000] = {0};
	char path2[1000];
	myfile *p, *temp;
	chdir(path);
	printf("path :%s\n", path);
	getcwd(path2, 1000);
	//chdir(path2);
	dir = opendir(path2);
	while((ptr = readdir(dir)) != NULL)
	{
		lstat(ptr->d_name, &buf);
		if(S_ISDIR(buf.st_mode))
		{
			if(strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
				continue;
			else
			{
				strcpy(pathname, path2);
				if(pathname[strlen(pathname) - 1] != '/')
					pathname[strlen(pathname)] = '/';
				strcat(pathname, ptr->d_name);
				//printf("pathname: %s\n", pathname);
				display(pathname, head);
				memset(pathname, '\0', 1000);
				chdir("../");
			}
		}
		else
		{	
			p = (myfile *)malloc(sizeof(myfile));
			p->fid = ++count;
			strcpy(p->filename, ptr->d_name);
			//printf("%d\n", count);
			//printf("%s\n", p->filename);
			print(ptr->d_name, p);
			//printf("%d\n", p->mode);
			//printf("%d\n", p->size);
			if(*head == NULL)
			{
				*head = p;
				temp = p;
				temp->next = NULL;
			}
			else
			{
				temp->next = p;
				p->next = NULL;
				temp = p;
				temp->next = NULL;
			}
			int fd2;
			fd2 = open("/home/xuexi/summer/TeamB/zhuwenbo/week1/allfies.dat", O_CREAT, O_WRONLY);
			if(count != 7 || count != 28)
				write(fd2, p->data, 256);
			if(count == 18 || count == 20)
				p->mode = 777;
			close(fd2);
		}
	}
}

int main(int argc, char *argv[])
{
	myfile *files = NULL;
	display(argv[argc - 1], &files);
	myfile *temp = files;
	
	while(temp != NULL)
	{
		printf("fid :%d\n", temp->fid);
		printf("filename: %s\n", temp->filename);
		printf("mode: %d\n", temp->mode);
		printf("size: %d\n", temp->size);
		printf("data: %s\n", temp->data);
		temp = temp->next;
	}
	
	return 0;
}


