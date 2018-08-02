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
int main(void)
{

	char c;
	while(	c = getch())  {
		if(c == 65)
			printf("up");
		if(c == 66)
			printf("down");
		if(c == 67)
			printf("right");
		if(c == 68)
			printf("left");
		if(c == 'q')
			break;
	}
	return 0;
}
