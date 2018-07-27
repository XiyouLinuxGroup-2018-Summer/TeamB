#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int my_read(const char *path)
{
	DIR *dir;
	struct dirent *ptr;

	if((dir= opendir(path)) == NULL)
	{
		perror("dir");
		exit(1);	
	}
	while((ptr=readdir(dir))!=NULL)
	{
		printf("file name: %s\n",ptr->d_name);
	}
	closedir(dir);
	return 0;
}

int main(int argc, char **argv)
{
	if(argc<2)
	{
		printf("listfile <target path>\n");
		exit(1);
	}
	if(my_read(argv[1])<0)
	{
		exit(1);
	}
	return 0;
}
