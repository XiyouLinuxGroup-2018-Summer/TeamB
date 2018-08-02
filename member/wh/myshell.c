#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<dirent.h>
#include<termio.h>

//实现无缓冲输入
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

//用于打印出当前目录
void print_prompt(void) {
	char cata[30];
		getcwd(cata,30);						//获取当前目录
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

void comple(char *ch)	//补全命令
{
	DIR *dir;
	struct dirent *ptr;
	char c;
	int count = 0;
	char str[20][50];
	int i;						//循环变量
	//打开存放命令文件的目录名的文件
	int cnt = 0;
	int fd;
	char pt[50] = "";											//用于存储目录文件名
	if((fd = open("1.txt",O_RDONLY)) == -1) {				//打开存放目录文件名的文件，利用echo $PATH >> 1.txt重定向输出得到
		perror("open:");
		return;
	}
	while(read(fd,&c,sizeof(char)) > 0) {
		if(c != ':')
			pt[cnt++] = c;										//拼接目录文件名
		else {													//得到目录文件名，打开目录文件
			if((dir = opendir(pt)) == NULL) {					//重复上述比较步骤，得到补全的命令名
				perror("");
				return;
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
	putchar('\n');
	for(i = 0;i < count;i++)
		printf("%15s",str[i]);	
	putchar('\n');
}
//获取用户输入
void get_input(char *buf)
{
	int count = 0;
	int len = 0;
	char c;
	char ch[50];				//存放输入的命令
	for(int i = 0;i < 50;i++)
		ch[i] = '\0';
	while(c = getch()) {
		switch(c) {
			case '	':
				comple(ch);	//补全命令
				print_prompt();
				printf("%s",ch);
				continue;
				break;
			case 65:
			//	void history();		//历史命令
				continue;
				break;
			case 127:
				putchar('\b');		//回删
				ch[strlen(ch) - 1] = '\0';
				count--;
				continue;
				break;
			case '\r':				//回车命令输入结束
				strcpy(buf,ch);
				return;				//结束输入
		}
		ch[count++] = c;
	}
}

//解析buf中的命令,将结果存入arglist中，命令以回车符号\r结束(已自动转换为\0)
void explain_input(char *buf,int *argcount,char arglist[100][256])
{	
	int i,j;
	char *p = buf;
	for(i = 0;i < strlen(p);i++) {
		if(p[i] == ' ') {
			strncpy(arglist[(*argcount)++],p,i);
			for(j = 0;j <= i;j++,p++);
			i = 0;
		}
		if(p[i+1] == '\0') {
			strncpy(arglist[(*argcount)++],p,i+1);
			break;
		}
		
	}
}



int main(void)
{
	char buf[50];			//存放输入的命令
	char arglist[100][256];	//存放解析过后的命令
	int argcount;			
	int i;					//循环变量
	
	print_prompt();
	get_input(buf);
	argcount = 0;
	explain_input(buf,&argcount,arglist);	//解析命令

	return 0;
}
