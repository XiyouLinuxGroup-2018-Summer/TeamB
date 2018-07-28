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
 	char filename[128];
	int mode;					// 直接使用整型存储的文件权限(例如 755 存储为 493 也就是 0755)
    int size; 					// 大小
    char data[256]; 			// 文件内容
	struct file *next;
}myfile;


void print(myfile *files)
{       
		int fd;
        int mode;
		int mode_u = 0, mode_g = 0, mode_o = 0;
        char buf_time[32];
		struct stat buf;
		fd = open(files->filename, O_RDONLY, S_IRUSR|S_IWUSR);
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
		files->mode = mode_u * 8 * 8 + mode_g * 8 + mode_o;
         files->size  = buf.st_size;
}

void display(char *path, myfile **files)
{
	//printf("path :%s\n", path);
	static int count = 0;
	struct stat buf;
	DIR *dir;
	struct dirent *ptr;
	char pathname[1000] = {0};
	char path2[1000];
	myfile *node;
	node = (myfile *)malloc(sizeof(myfile));
	//files = node;
	node->next = *files;
	chdir(path);
	getcwd(path2, 1000);
	dir = opendir(path2);
	while((ptr = readdir(dir)) != NULL)
	{
		lstat(ptr->d_name, &buf);
		if(S_ISDIR(buf.st_mode))
		{
			strcpy(pathname, path);
			if(pathname[strlen(pathname) - 1] != '/')
				pathname[strlen(pathname)] = '/';
			strcat(pathname, ptr->d_name);
			display(pathname, &node);
			chdir("../");
		}
		else
		{	
			count++;
			node->fid = count;
			strcpy(node->filename, ptr->d_name);
			print(node);
		}
	}
}

int main(int argc, char *argv[])
{
	myfile *files;
	files = (myfile *)malloc(sizeof(myfile));
	files->next = NULL;
	display(argv[argc - 1], &files);
	printf("%s\n", files->filename);
	printf("%s\n", files->data);
	return 0;
}


