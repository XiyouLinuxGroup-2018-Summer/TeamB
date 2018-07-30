#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
int my_readir(const char *path)
{
	DIR *dir;
	struct dirent *ptr;
	if((dir = opendir(path)) == NULL) {
		printf("open the %s failed",path);
		return -1;
	}

	while((ptr = readdir(dir)) != NULL) {
		printf("%s  ",ptr->d_name);
	}
	closedir(dir);
	return 0;
}

int main(int argc,char *argv[])
{
	if(argc < 2) {
		printf("错误的引用");
		return 0;
	}
	if(my_readir(argv[1]) < 1)
		return -1;
	return 0;
}
