/*2018-7-23 周一 */
/*my_rwl.c 文件读写和文件读写指针的移动操作过程 */
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

void LSEEK(int fd,size_t size,int fp) {
	char str;
	if(read(fd,&str,size) >= 1)	
		LSEEK(fd,size,fp);
	write(fp,&str,sizeof(str));
}

const char *PAR_FILE = "par.txt";
const char *SWEP_FILE = "swep.txt";
int main(void)
{
	int fp  = open(PAR_FILE,O_RDWR);
	if(fp == -1) {
		printf("error");
		return 0;
	}
	int fd  = open(SWEP_FILE,O_RDWR);
	if(fd == -1) {
		printf("error");
		return 0;
	}

	for(char a = 'a';a < 'z';a++)		//在临时文件中写入数据
		if(write(fd,&a,sizeof(a)) == -1)
			printf("error,no data to write\n");
	
	lseek(fd,0,SEEK_SET);
	char c;
	LSEEK(fd,sizeof(c),fp);
	
	close(fd);
	close(fp);
	return 0;
}
