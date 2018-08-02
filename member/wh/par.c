#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termio.h>
#include<unistd.h>
#include<dirent.h>
#include<stdlib.h>
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
int main(void)
{
	char ch;
	while(ch = getch() ) {
		switch(ch) {
			case 65:
				printf("up");
				break;
			case 66:
				printf("down");
				break;
			case 67:
				printf("left");
				break;
			case 68:
				printf("right");
				break;
		}
		if(ch == 'q')
			break;
	}
	return 0;
}
