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
	
	if(count == 0)
		printf("no file!!!");
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

