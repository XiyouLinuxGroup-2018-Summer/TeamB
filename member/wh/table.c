#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termio.h>
#include<unistd.h>
#include<dirent.h>
int getch(void)
{
     struct termios tm, tm_old;
     int fd = 0, ch;
 
     if (tcgetattr(fd, &tm) < 0) {//保存现在的终端设置
          return -1;
     }
 
     tm_old = tm;
     cfmakeraw(&tm);//更改终端设置为原始模式，该模式下所有的输入数据以字节为单位被处理
     if (tcsetattr(fd, TCSANOW, &tm) < 0) {//设置上更改之后的设置
          return -1;
     }
 
     ch = getchar();
	 printf("%c",ch);
     if (tcsetattr(fd, TCSANOW, &tm_old) < 0) {//更改设置为最初的样子
          return -1;
     }
    
     return ch;
}
void print_prompt(void) {
	char cata[20];
		getcwd(cata,20);						//获取当前目录
		int k;										//若为主目录，则换成~
		for(k = 0;k < strlen(cata);k++)
			if(!strncmp(cata,"/home/wh",7)) {
				cata[0] = '~';
				int m = strlen(cata);
				for(k = 1;k < strlen(cata) - 7;k++)
					cata[k] = cata[k+7];
				cata[m - 7] = '\0';
			}
	
		char c;
	    char ch[50];
	    int count = 0;
		printf("\033[35m\33[1m");
	    printf("\033[35m20:17\33[1m-\033[32mwh@wh-ST-Plus-KN\33[1m\033[37m:\33[1m\033[34m%s\33[1m\033[37m$\33[1m ",cata);
		printf("\033[37m\33[1m");	
}
    
int main(void)
{
	print_prompt();
    int count = 0;
	char c;
	char ch[50];
	while(c = getch() ) {
        
        if(c == '	') 
            break;
        ch[count++] = c;
		
    }
	ch[count++] = '\0';
    char str[20][20];						//用于存储所有补全文件
    int i;                                                      //循环变量
	count = 0;
	struct dirent *ptr;

	DIR *dir;

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

