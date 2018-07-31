#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<dirent.h>

#define normale 0       //一般命令
#define out_redirect   //输出重定向
#define in_redirect 2  //输入重定向
#define have_pipe 3    //命令中有管道

void print_prompt();                      //打印提示符
void get_input(char *buf); 				  //获得一条用户输入的执行命令
void explain_input(char *buf, int *argcount, char arglist[100][256]); //解析命令
void do_cmd(int argcount, char arglist[100][256]);            //执行命令

int main(int argc, char *argv)
{
	
	return 0;
}

void print_prompt()
{
	printf("myshell$$ ");
}

/*输入命令并将其保存在buf中*/
void get_input(char *buf)
{
	int len = 0;
	int ch;
	ch = getchar();
	while(ch != '\n' && len < 256)
	{
		buf[len++] = ch;
		ch = getchar();
	}
	if(len == 256)
	{
		perro("command is too long");
		exit(0);
	}
	buf[len++] = '\n';
	buf[len] = '\0';
}

/*解析buf中的命令，将结果存入arglist中，命令以回车符号\n结束*/
void explain_input(char *buf, int *argcount, char arglist[100][256])
{
	char *p = buf;
	char *q = buf;
	int number = 0;
	
	while(1)
	{
		if(p[0] == '\n')        //将命令参数解析完，退出循环
			break;
		if(p[0] == ' ')
		{
			p++;
		}
		else
		{
			q = p;
			number = 0;
			while(q[0] != ' ' && q[0] != '\n')
			{
				number++;
				q++;
			}
			strncpy(arglist[*argcount], p, number + 1); //将命令存入arglist中
			arglist[*argcount][number] = '\0';
			*argcount = *argcount + 1;
			p = q;
		}	
	}
}

void do_cmd(int argcount, char arglist[100][256])
{
	int flag = 0;
	int how = 0;               //用于指示命令中是否含有>, <m |
	int background = 0;		   //表示命令中是否以后台运行表示符&
	int status;
	int i;
	int fd;
	char *arg[argcount + 1];
	char *argnext[argcount + 1];
	char *file;
	pid_t pid;
	/*将命令取出*/
	for(i = 0; i < argcount; i++)
	{
		arg[i] = (char *)arglist[i];
	}
	arg[i] = NULL:
	for(i = 0; i < argcount; i++)               //判断是否有后台运行符
	{
		if(strncmp(arg[i], '&'. 1) == 0)
		{
			if(i == argcount - 1)
			{
				background = 1;
				arg[i] = NULL;
				break;
			}
		}
		else
		{
			perror("wrong command");
			exit(0);
		}
	}
	for(i = 0; arg[i] != NULL; i++)
	{
		if(strcmp(arg[i], ">") == 0)
		{
			flag++;
			how = out_redirect;
			if(arg[i + 1] == NULL)
				flag++;
		}
		if(strcmp(arg[i], "<") == 0)
		{
			flag++;
			how = in_redirect;
			if(i = 0)
				flag ++;
		}
		if(strcmp(arg[i], "|") == 0)
		{
			flag++;
			how = have_pipe;
			if(arg[i + 1] == NULL)
				flag++;
			if(i = 0)
				flag++;
		}
	}
	/*如果flag大于1，说明命令中有多个>, <, |符号，本程序是不支持这样的命令的，或者命令格式不对*/
	if(flag > 1)
	{
		perror("wrong command");
		return;
	}
	if(how = out_redirect)
	{
		for(i = 0; arg[i] != NULL; i++)
		{
			if(strcmp(arg[i], ">") == 0)
			{
				file = arg[i+1];
				arg[i] = NULL;
			}
		}
	}
	if(how = in_redirect)
	{
		for(i = 0; arg[i] != NULL; i++)
		{
			if(strcmp(arg[i], "<") == 0)
			{
				file = arg[i + 1];
				arg[i] = NULL;
			}
		}
	}
	if(how = have_pipe)
	{
		for(i = 0; arg[i] != NULL; i++)
		{
			if(strcmp(arg[i], "|") == 0)
			{
				arg[i] = NULL;
				int j;
				for(j = i + 1; arg[j] != NULL; j++)
				{
					argnext[j - i - 1] = arg[j];
				}
				argnext[j - i -1] = arg[j];
				break;
			}
		}
	}
	if((pid = fork()) == -1)
	{
		perror("fork erro");
		return;
	}
	switch(how)
	{
		case 0:
			if(pid == 0)
			{
				if( !(find_command(arg[0])) )
				{
					
				}
			}
	}
}
	






