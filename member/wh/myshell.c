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
#include<readline/readline.h>
#include<readline/history.h>
int find_command(char *command);
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

void comple(char *ch,int flag)	//补全命令
{
	
	DIR *dir;
	struct dirent *ptr;
	char c;
	int count = 0;
	char str[20][50];
	char *p = ch;
	int i;						//循环变量
	for(i = 0;(ch)[i] != ' ';i++,p++);
	p++;
	if(flag == 0) {				//命令补全
		//打开存放命令文件的目录名的文件
		int cnt = 0;
		int fd;
		char pt[50] = "";											//用于存储目录文件名
		if((fd = open("/home/wh/wh2/TeamB/member/wh/1.txt",O_RDONLY)) == -1) {				//打开存放目录文件名的文件，利用echo $PATH >> 1.txt重定向输出得到
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
		close(fd);
		if(count > 1) {
			putchar('\n');
			for(i = 0;i < count;i++)
				printf("%15s",str[i]);	
			putchar('\n');
		}
	/*	else if(count == 1) {
			for(i = strlen(ch);i < strlen(str[0]);i++)
				putchar(str[0][i]);
		}
	*/	
			
	}
	else {					//文件补全
		char cata[30];
		getcwd(cata,30);
		if((dir = opendir(cata)) == NULL) {
			perror("");
			return;
		}
		while((ptr = readdir(dir)) != NULL) {
			if(!strncmp(p,ptr->d_name,strlen(p)))
				strcpy(str[count++],ptr->d_name);	
		}
		closedir(dir);
		putchar('\n');
		for(i = 0;i < count;i++)
			printf("%15s",str[i]);
		putchar('\n');
	}
}
//显示历史命令
void history()
{
	int fd;
	if((fd = open("history.dat",O_RDONLY)) == -1) {
		perror("");
		return;
	}
	lseek(fd,50,SEEK_END);
	int i;
	char ch[50];
	for(i = 0;i < 50;i++)
		ch[i] = '\0';
	int count = 0;
	while(read(fd,ch,50) != -1)
		
	close(fd);
	printf("%s\n",ch);

}



//获取用户输入
void get_input(char *buf)
{
	int num;				//记录最后一个输入命令的长度
	int fp;
	int count = 0;
	int len = 0;
	char c;
	char ch[50];				//存放输入的命令
	int flag = 0;					//判断是文件补全还是命令补全
	for(int i = 0;i < 50;i++)
		ch[i] = '\0';
	while(c = getch()) {
		switch(c) {
			case '	':
				comple(ch,flag);	//补全
				print_prompt();
				printf("%s",ch);
				continue;
				break;
			case 65:
			case 66:
				//				history();		//历史命令
				continue;
				
			case 127:
				if(count > 0) {
					putchar('\b');		//回删
					ch[strlen(ch) - 1] = '\0';
					count--;
				}
				continue;
				break;
			case '\r':				//回车命令输入结束
				strcpy(buf,ch);
		//		if((fp = open("history.dat",O_RDWR | O_APPEND)) == -1) {
		//			perror("");
		//			return;
		//		}
		//		write(fp,ch,sizeof(char)*50);
				close(fp);
				return;				//结束输入
		}
		if(c == ' ')
			flag = 1;				//此时标记为文件补全
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

//根据分离出的结果执行命令
void do_cmd(int argcount,char arglist[100][256])
{
	if(strcmp(arglist[0],"cd") == 0) {
		chdir(arglist[1]);
		return;
	}
	int flag = 0;
	int how = 0;		//用于指示命令中是否有 >,<,|
	int background = 0;	//标识，命令中是否有后台运行标识符
	int status;
	int i;
	int fd;
	char *arg[argcount+1];
	char *argnext[argcount+1];
	char *file;
	pid_t  pid;
	//将命令取出
	for(i = 0;i < argcount;i++)
		arg[i] = (char *)arglist[i];

	arg[argcount] = NULL;
	//查看命令中是否有后台运行程序
	for(i = 0;i < argcount;i++) {
		if(strncmp(arg[i],"&",1) == 0) {
			if(i == argcount-1) {
				background = 1;
				arg[argcount-1] = NULL;
				break;
			}
			else {
				printf("wrong command\n");
				return;
			}
		}
	}
	int cnt = 0;
	for(i = 0;arg[i] != NULL;i++) {
		if(strcmp(arg[i],">") == 0) {
			flag = 0001;
			cnt = 1;
			if(arg[i+1] == NULL)
				flag = 0002;
		}
		if(strcmp(arg[i],">>") == 0) {
			flag = 0001;
			cnt = 2;
			if(arg[i+1] == NULL)
				flag = 0002;
		}
	
		if(strcmp(arg[i],"<") == 0) {
			
			flag = 0010;
			if(i == 0)
				flag = 0020;
		}
		if(strcmp(arg[i],"|") == 0) {
			flag = 0100;
			if(arg[i+1] == NULL)
				flag = 0200;
		}
	}
	
	if(flag == 0002 && flag == 0020 && flag == 0200) {
		printf("wrong command");
		return;
	}

	if(flag == 0001) {
		for(i = 0;arg[i];i++) {
			if(strcmp(arg[i],">") == 0) {
				file = arg[i+1];
				arg[i] = NULL;
				break;
			}
			if(strcmp(arg[i],">>") == 0) {
				file = arg[i+1];
				arg[i] = NULL;
				break;
			}
		}
	}
	if(flag == 0010) {
		for(i = 0;arg[i];i++)
			if(strcmp(arg[i],"<") == 0) {
				file = arg[i+1];
				arg[i] = NULL;
			}
	}

	
	//命令只含有一个管道符号
	//把管道符号后面的部分存入argnext中，管道后面的部分是一个可执行的shell命令
	if(flag == 0100) {
		for(i = 0;arg[i];i++)
			if(strcmp(arg[i],"|") == 0) {
				arg[i] = NULL;
				int j;
				for(j = i+1;arg[j];j++)
					argnext[j-i-1] = arg[j];
				argnext[j-i-1] = arg[j];
				break;
			}
	}

	if((pid = fork()) < 0) {
		printf("fork error\n");
		return;
	}
	switch(flag) {
		//无<,>,|
		case 0000:
			if(pid == 0) {
				if(!find_command(arg[0])) {
					printf("%s : command not found\n",arg[0]);
					exit(0);
				}
				execvp(arg[0],arg);
			}
			break;
		case 0001:
			//含有>
			if(pid == 0) {
				if(!find_command(arg[0])) {
					printf("%s : command not found\n",arg[0]);
					exit(0);
				}
				if(cnt == 1)
					fd = open(file,O_RDWR | O_CREAT | O_TRUNC,0644);
				else
					fd = open(file,O_RDWR | O_CREAT | O_APPEND,0644);
				dup2(fd,1);
				execvp(arg[0],arg);
				exit(0);
			}
			break;
		case 0010:
			//含有<
			if(pid == 0) {
				if(!find_command(arg[0])) {
					printf("%s : command not found\n",arg[0]);
					exit(0);
				}
				fd = open(file,O_RDONLY);
				dup2(fd,0);
				execvp(arg[0],arg);
				exit(0);
			}
			break;
		case 0100:
			//含有管道符
			if(pid == 0) {
				int pid2;
				int status2;
				int fd2;
				if((pid2 = fork()) < 0) {
					printf("fork2 error\n");
					return;
				}
				else if(pid2 == 0) {
					if(!(find_command(arg[0]))) {
						printf("%s : command not found\n",arg[0]);
						exit(0);
					}
					fd2 = open("/tmp/youdonotknowfile",O_WRONLY | O_CREAT | O_TRUNC,0644);
					dup2(fd2,1);
					execvp(arg[0],arg);
					exit(0);
				}
				if(waitpid(pid2,&status2,0) == -1) 
					printf("wait for child process error\n");
				if(!(find_command(argnext[0]))) {
					printf("%s : command not found\n",argnext[0]);
					exit(0);
				}
				fd2 = open("/tmp/youdonotknowfile",O_RDONLY);
				dup2(fd2,0);
				execvp(argnext[0],argnext);
		//		if(remove("/tmp/youdonotknowfile"))
				exit(0);
			}
				break;
			default:
				break;
	}
	
	//若命令中有&，表示后台执行，父进程直接返回，不等待子进程结束
	if(background == 1) {
		printf("[process id %d]\n",pid);
		return;
	}

	//父进程等待子进程结束
	if(waitpid(pid,&status,0) == -1)
		printf("wait for child process error\n");


}

//查找命令中的可执行程序
int find_command(char *command)

{
	DIR *dp;
	struct dirent *dirp;
	if(strcmp(command,"cd") == 0)
		return 1;
	char *path[] = {"./","/bin","/usr/bin",NULL};
	if(strncmp(command,"./",2) == 0)
		command = command + 2;
	int i = 0;
	while(path[i] != NULL) {
		if((dp = opendir(path[i])) == NULL)
			printf("can not open");
		while((dirp = readdir(dp)) != NULL) {
			if(strcmp(dirp->d_name,command) == 0) {
				closedir(dp);
				return 1;
			}
		}
		closedir(dp);
		i++;
	}
	return 0;
}
/*
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
		return 0;
	}
	while(read(fd,&c,sizeof(char)) > 0) {
		if(c != ':')
			pt[cnt++] = c;										//拼接目录文件名
		else {													//得到目录文件名，打开目录文件
			if((dir = opendir(pt)) == NULL) {					//重复上述比较步骤，得到补全的命令名
				perror("1");
				return 0;
			}
			while((ptr = readdir(dir)) != NULL) {
				if(!strncmp(command,ptr->d_name,strlen(command)))
					return 1;
			}
		
			closedir(dir);
		}
	}
	close(fd);
	return 0;
}
*/
int main(void)
{
	char buf[50];			//存放输入的命令
	char arglist[100][256];	//存放解析过后的命令
	int argcount;			
	int i,j;					//循环变量
	 
	while(1) {
		for(i = 0;i < 50;i++)
			buf[i] = '\0';
		for(i = 0;i < 100;i++)
			for(j = 0;j < 256;j++)
				arglist[i][j] = '\0';
		print_prompt();
		signal(SIGINT,SIG_IGN);
		char *str = readline(" ");
		add_history(str);
		strcpy(buf,str);
		//get_input(buf);
		if(strcmp(buf,"exit") == 0 || strcmp(buf,"logout") == 0)
			break;
		argcount = 0;
		putchar('\n');
		explain_input(buf,&argcount,arglist);	//解析命令
		do_cmd(argcount,arglist);
	}
	exit(0);
}
