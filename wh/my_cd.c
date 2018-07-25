#include<stdio.h>
#include<unistd.h>
//#include<Linux/limits.h>

int main(int argc,char *argv[])
{
	char buf[512];
	if(argc < 2) {
		printf("错误的调用");
		return 0;
	}

	if(chdir(argv[1]) < 0) {
		printf("chdir error");
	}

	if(getcwd(buf,512) < 0) {
		printf("buf error");
	}

	printf("%s\n",buf);

	return 0;
}
