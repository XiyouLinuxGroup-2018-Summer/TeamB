#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termio.h>
#include<unistd.h>
#include<dirent.h>
#include<stdlib.h>
int main(void)
{

    char ch[20];
    char str[20][20];						//用于存储所有补全文件
    int i;                                                      //循环变量
    int count = 0;                                              //计数器

	struct dirent *ptr;
    DIR *dir;
    printf("20:17-wh@wh-ST-Plus-KN:~$ ");
    scanf("%s",ch);

    if((dir = opendir(".")) == NULL) {
        perror("");
        return 0;
    }
    while((ptr = readdir(dir)) != NULL)
        if(!strncmp(ch,ptr->d_name,strlen(ch)))                         //与输入的字符之间进行判断
                strcpy(str[count++],ptr->d_name);           //若相同，则计数并存储起来
	closedir(dir);
	//打开存放命令文件的目录名的文件
	int cnt = 0;
	int fd;
	char c;													//用于读取文件
	char pt[50] = "";											//用于存储目录文件名
	if((fd = open("1.txt",O_RDONLY)) == -1) {				//打开存放目录文件名的文件，利用echo $PATH >> 1.txt重定向输出得到
		perror("open:");
		return 0;
	}
	while(read(fd,&c,sizeof(char)) > 0) {
		if(c != ':')
			pt[cnt++] = c;										//拼接目录文件名
		else {													//得到目录文件名，打开目录文件
			if((dir = opendir(pt)) == NULL) {					//重复上述比较步骤，得到补全的命令名
				perror("");
				return 0;
			}
			while((ptr = readdir(dir)) != NULL) {
				if(!strncmp(ch,ptr->d_name,strlen(ch)))
					strcpy(str[count++],ptr->d_name);
			}
			int length = strlen(pt);
			for(i = 0;i < length;i++)
				pt[i] = '\0';
			closedir(dir);
			cnt = 0;
		}
	}
	close(fd);
	
	
	if(count == 0)
		printf("no file or command!!!");
	if(count == 1) {
            printf("20:17-wh@wh-ST-Plus-KN:~$ ");
				puts(str[0]);
	}
            
    else
            for(i = 0;i < count;i++)
                printf("%s\t",str[i]);
				putchar('\n');

    return 0;
}

