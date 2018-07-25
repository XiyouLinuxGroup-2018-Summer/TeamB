#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
int main(int argc,char *argv[])
{
	if(argc < 3) {
		printf("错误的引用");
		return 0;
	}

	int i,j;	//循环变量
	int cnt = 0;
	for(i = 0;i < strlen(argv[1]);i++)
		if(argv[1][i] == ':') {
			cnt = 1;
			break;
		}

	char owner[strlen(argv[1])];
	char group[strlen(argv[1])];
	if(cnt) {
		for(j = 0;j < strlen(argv[1])-1;j++) {
			if(j < i)
				owner[j] = argv[1][j];
			else
				group[j-i] = argv[1][j+1];
		}
		owner[i] = '\0';
		group[strlen(argv[1]) - i - 1] = '\0';
	}
	else {
		strcpy(owner,argv[1]);
	}
	//	printf("\nowner = %s,group = %s\n",owner,group); //测试是否成功提取信息
	struct stat owner_buf;
	struct stat group_buf;
	if(stat(owner,&owner_buf) == -1) {
		printf("未存在该用户");
		return 0;
	}

		
//	chown(argv[2],owner,group);
	return 0;
}

