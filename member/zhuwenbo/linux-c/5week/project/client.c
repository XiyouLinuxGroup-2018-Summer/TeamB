#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<pthread.h>
/*客户端*/
#define PORT 8848           //端口地址
#define send_length 200     
#define CLOSE printf("\033[0m");     //关闭彩色字体                         
#define GREEN printf("\e[1;32m");      //绿色字体
#define RED printf("\e[1;31m");        //红色字体
#define CLEAR printf("\033[2J");       //清屏
#define YELLOW printf("\e[1;33m");     //黄色字体
#define BLUE printf("\e[1;34m");       //蓝色字体

char *IP = "128.0.0.208";
/*错误处理函数*/
void my_err(char *str, int line)
{
	fprintf(stderr, "line:%d\n", line);
	perror(str);
	exit(0);
}
/*注册*/
int user_login(int socket)	
{
	char use[200] = {0};
	char username[21] = {0};
	char passwd[21] = {0};
	char ans[21] = {0};
	char flag[200] = {0};
	int recv_length = 200;
	printf("----------------------\n");
	printf("\t\t注册\n");
	printf("用户名长度小于20\n");
	printf("请输入要注册的用户名：");
	getchar();
	scanf("%[^\n]", username);
	username[strlen(username)] = ':';     //分隔名字，密码，密保问题的分隔符
	username[strlen(username)] = '\0';

	printf("请输入密码，长度小于20: ");
	getchar();
	scanf("%[^\n]", passwd);
	passwd[strlen(passwd)] = ':';
	passwd[strlen(passwd)] = '\0';

	printf("密保问题: 你帅不帅\n");
	getchar();
	scanf("%[^\n]", ans);
	ans[strlen(ans)] = '\0';
	strcpy(use, "login:");
	strcat(use, username);
	strcat(use, passwd);
	strcat(use, ans);
	if(send(socket, use, send_length, 0) < 0)
		my_err("send", __LINE__);
	int len, count = 0;
	while( (len = recv(socket, flag, recv_length, 0)) )
	{
		count += len;
		if(count == 200)
			break;
		else
			recv_length -= len;
	}
	if(len == -1)
		my_err("recv", __LINE__);
	if(strncmp(flag, "success", 7) == 0)
	{
		printf("注册成功\n");
		printf("----------------------\n");
		return 1;
	}
	else
		return 0;
}
/*登录*/
int user_enter(int socket)
{
	char use[200] = "enter:";
	char name[21] = {0};
	char passwd[20] = {0};
	char flag[200] = {0};
	int recv_length = 200;      
	printf("------------------\n");
	printf("登录界面\n");
	printf("请输入你的用户名：");
	getchar();
	scanf("%[^\n]", name);
	name[strlen(name)] = ':';
	name[strlen(name)] = '\0';
	printf("请输入你的密码：");
	getchar();
	scanf("%[^\n]", passwd);
	passwd[strlen(passwd)] = '\0';
	strcat(use, name);
	strcat(use, passwd);
	if(send(socket, use, send_length, 0) < 0)
		my_err("send", __LINE__);
	int len, count = 0;
	while( (len = recv(socket, flag, recv_length, 0)) )
	{
		count += len;
		if(count == 200)
			break;
		else
			recv_length -= len;
	}
	if(len == -1)
		my_err("recv", __LINE__);
	if(strncmp(flag, "success", 7) == 0)
	{
		printf("登录成功\n");
		printf("------------------\n");
		return 1;
	}
	else
		return 0;
}
//找回密码
int find_passwd(int socket)
{
	char answer[200] = "find_passwd:";
	char name[21] = {0};								//用户名
	char ans[21] = {0};									//回答
	char passwd[200] = {0};                    		    //找回的密码
	printf("-------------------\n");
	printf("找回密码\n");
	printf("请输入你的用户名\n");
	getchar();
	scanf("%[^\n]", name);
	name[strlen(name)] = ':';
	name[strlen(name)] = '\0';
	printf("问题：你帅不帅\n");
	getchar();
	scanf("%[^\n]", ans);
	/*发送请求及用户名和答案*/
	strcat(answer, name);           
	strcat(answer, ans);		
	if(send(socket, answer, send_length, 0) < 0)
		my_err("send", __LINE__);
	int len, count = 0;
	int recv_length = 200;
	while( (len = recv(socket, passwd, recv_length, 0)) )
	{
		count += len;
		if(count == 200)
			break;
		else
			recv_length -= len;
	}
	if(len == -1)
		my_err("recv", __LINE__);
	if(strncmp(passwd, "fault", 5))
	{
		//strcpy(passwd, flag);
		printf("回答正确，您的密码是：%s\n", passwd);
		printf("---------------\n");
		return 1;
	}
	else
		return 0;
}
/*是否添加好友*/
void add_friend(char *str, int socket)
{
	char ch;
	char flag[200] = {0};
	int count = 0, j = 0, k = 0;
	char name[20] = {0};
	char message[200] = {0};
	for(int i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == ':')
		{
			count++;
			i++;
			k = i;
		}
		if(count == 1)
		{
			if(str[i] ==' ')
				break;	
			name[j++] = str[i];
		}
	}
	strcpy(message, &str[k]);
	printf("好友添加请求 %s\n", message);
	printf("是否同意，同意请输入@agree:加对方姓名， 不同意请输入@disagree:加对方姓名\n");
}

/*查看好友状态信息*/
void watch_friend(char *str)
{
	int i, j = 0, count = 0;
	int len = 0;
	char name[200] = {0};
	char message[200] = {0};
	int flag;
	for(i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == ':')
		{
			count++;
			i++;
		}
		if(str[i] == ' ')
		{
			count++;
			i++;
		}
		if(count == 1)
		{
			name[j++] = str[i];               //获取用户名
		}
		if(count == 2)
		{
			len = strlen(name);
			for(j = 0; j < 30 - len; j++)
				message[j] = ' ';
			if(strcmp(&str[i], "online") == 0)
				flag = 0;
			else
				flag = 1;
			strcat(message, &str[i]);
			break;
		}
	}
	if(flag == 0)
	{
		printf("\t%s", name);
		GREEN
		printf("%s\n", message);
		CLOSE
	}
	else
	{
		printf("\t%s", name);
		RED
		printf("%s\n", message);
		CLOSE
	}
}

/*接收离线消息*/
void recv_record(char *str)
{
	int i, j = 0, count = 0;
	char message[200] = {0};
	for(i = 0; str[i] != 0; i++)
	{
		if(str[i] == ':')
			count++, i++;
		if(count == 1)
			message[j++] = str[i];
	}
	YELLOW
	printf("您有离线消息: %s\n", message);
	CLOSE
}

/*查看历史记录*/
void recv_history(char *str)
{
	int i = 0, j = 0, count = 0;
	char message[200] = {0};
	for(; str[i] != '\0'; i++)
	{
		if(str[i] == ':' && count < 1)
			count++, i++;
		if(count == 1)
			message[j++] = str[i];
	}
	printf("%s\n", message);
}

/*创建群聊*/
void create_group(int socket)
{
	int len = 0, count = 0, recv_length = 200;
	printf("\t   创建群聊 \n");
	printf("请输入想要创建的群名字,长度小于20: ");
	char name[20] = {0};
	char recv_buf[200] = {0};                         //接收服务器返回的消息
	getchar();
	scanf("%[^\n]", name);
	char message[200] = "!create:";
	strcat(message, name);
	printf("create message = %s\n", message);
	if(send(socket, message, send_length, 0) < 0)
		my_err("send", __LINE__);
	while(len = recv(socket, recv_buf, recv_length, 0) < 0)
	{
		if(count == 200)
			break;
		count += len;
		recv_length -= len;
	}
	if(strncmp(recv_buf, "success", 7) == 0)
		printf("恭喜你创建成功\n");
	else
		printf("创建失败\n");
}

/*加群*/
void add_group(char *str)
{
	char message2[100] = {0};
	char group[20] = {0};
	int i, j = 0, count = 0;
	for(i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == ':' && count < 1)
			count++, i++;
		if(count == 1)
			message2[j++] = str[i];
	}
	printf("\t%s\n", message2);
	printf("是否同意该用户入群，同意请输入@@ag:加群名:加用户名，不同意请输入@@dis:加用户名\n");
}

void g_his(char *str)
{
	char message[200] = {0};
	strcpy(message, &str[6]);
	printf("%s\n", message);
}

/*查看群成员*/
void watch_member(char *str)
{
	int i, j = 0, count = 0;
	char name[20] = {0};
	char status[50] = {0};
	char message[30] = {0};
	for(i = 7; str[i] != '\0'; i++)
	{
		if(str[i] == ':' && count < 2)
			count++, i++;
		if(count == 1)
			name[j++] = str[i];
		if(count == 2)
		{
			strcpy(status, &str[i]);
			break;
		}
	}
	int len;
	if(strcmp(status, "online") == 0)
	{
		printf("%s", name);
		len = strlen(name);
		for(j = 0; j < 30 - len; j++)
			message[j] = ' ';
		GREEN
		
		printf("%s%s\n",message,status);
		CLOSE
	}
	else
	{
		printf("%s", name);
		len = strlen(name);
		for(j = 0; j < 30 - len; j++)
			message[j] = ' ';
		RED
		printf("%s%s\n",message, status);
		CLOSE
	}
}

void *send_message(void *arg)
{
	int socket = *(int *)arg;
	char message[200] = {0};
	char message1[200] = {0};
	while(1)
	{
		message[0] = '~';
		getchar();
		scanf("%[^\n]", message1);
		strcat(message, message1);
		/*查看好友列表*/
		if(strcmp(message, "~") == 0)
		{
			printf("-----------------------好友列表------------------------\n");
		}
		/*查看用户消息记录*/
		else if(strncmp(message, "~history:", 9) == 0)
		{
			GREEN
			printf("----------与用户%s消息记录---------\n", &message[9]);
			CLOSE
		}
		/*查看群消息记录*/
		else if(strncmp(message, "~g_his:", 7) == 0)
		{
			GREEN
			printf("--------群%s消息记录---------\n", &message[7]);
			CLOSE
		}
		/*创建群聊*/
		else if(strcmp(message, "~create") == 0)
		{
			create_group(socket);
			memset(message, 0, sizeof(message));
			memset(message1, 0, sizeof(message1));
			continue;
		}
		/*查看加入的群聊*/
		else if(strcmp(message, "~w") == 0)
		{
			BLUE
			printf("----------加入的群聊-----------\n");
			CLOSE
		}
		/*查看群成员*/
		else if(strncmp(message, "~w:", 3) == 0)
		{
			BLUE
			printf("----------群聊%s的成员为----------\n", &message[3]);
			CLOSE
		}
		/*聊天*/
		 if(send(socket, message, send_length, 0) < 0)
			my_err("send", __LINE__);
		memset(message, 0, sizeof(message));
		memset(message1, 0, sizeof(message1));
	}
}

void *recv_message(void *arg)
{
	int socket = *(int *)arg;
	char readbuf[200] = {0};
	int i, j, k;
	char name[20] = {0};
	char lastname[20] = {0};
	int len, count;
	int recv_length;
	while(1)
	{	
		len =  count = 0;
     	recv_length = 200;
		while( (len = recv(socket, readbuf, recv_length, 0)) )
		{
			count += len;
			if(count == 200)
				break;
			else
				recv_length -= len;
		}
		readbuf[strlen(readbuf)] = '\0';
		if(len == -1)
			my_err("recv", __LINE__);
		readbuf[strlen(readbuf)] = '\0';
		/*添加群聊*/
		if(strncmp(readbuf, "add_group", 9) == 0)
			add_group(readbuf);
		/*添加好友*/
		else if(strncmp(readbuf, "add", 3) == 0)
			add_friend(readbuf, socket);
		/*接收通知消息*/
		else if(strncmp(readbuf, "#", 1) == 0)          //通知
		{
			printf("----------------------------------------------------\n");
			BLUE
			printf("通知: %s\n", &readbuf[1]);
			CLOSE
			printf("----------------------------------------------------\n");
		}
		/*查看好友列表*/
		else if(strncmp(readbuf, "@friend:", 8) == 0)
			watch_friend(readbuf);
		/*接收离线消息*/
		else if(strncmp(readbuf, "off:", 4) == 0)
			recv_record(readbuf);
		/*查看历史记录*/
		else if(strncmp(readbuf, "history:", 8) == 0)
			recv_history(readbuf);
		/*查看群历史记录*/
		else if(strncmp(readbuf, "g_his:", 6) == 0)
			g_his(readbuf);
		/*查看群成员*/
		else if(strncmp(readbuf, "~member:", 8) == 0)
			watch_member(readbuf);
		else 
			printf("\t\t\t%s\n", readbuf);
		memset(readbuf, 0, sizeof(readbuf));
	}
}

int main(int argc, char *argv[])
{
	if(argc != 2)
		my_err("input erro", __LINE__);
	int cli_fd;
	int status;
	char message[1024] = {0};
	char message1[1024] = {0};
	struct sockaddr_in serv_addr;
	socklen_t len;
	int select, flag;
	if( (cli_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		my_err("socket", __LINE__);

	/*设置连接的结构*/
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	len = sizeof(struct sockaddr_in);
	if(connect(cli_fd, (struct sockaddr *)&serv_addr, len) < 0)
		my_err("connect", __LINE__);
	printf("请输入1(注册)，2(登录)或者3(找回密码)\n");
	scanf("%d", &select);
	switch(select)
	{
		case 1:
			while(!user_login(cli_fd))
				printf("注册失败，请重新注册\n");
			while(!user_enter(cli_fd))	
				printf("登录失败请重新登录\n");
			break;
		case 2:
			while(!user_enter(cli_fd))
				printf("登录失败，请重新登录\n");
			break;
		case 3:
			while(!find_passwd(cli_fd))
				printf("查找失败，请重新查找\n");
			while(!user_enter(cli_fd))	
				printf("登录失败请重新登录\n");
			break;
		default:
			printf("小伙子请输入正确的命令\n");
	}
	printf("=================================================\n");
	printf("添加好友在屏幕上输入     ");
	RED
	printf("add:加好友名字\n");
	CLOSE
	printf("删除好友在屏幕上输入     ");
	RED
	printf("rm:加好友名字\n");
	CLOSE
	printf("发送消息输入             ");
	RED
	printf("用户名:加消息\n");
	CLOSE
	printf("查看好友列表             ");
	RED
	printf("按回车即可\n");
	CLOSE
	printf("查看历史记录输入         ");
	RED
	printf("history:加想要查看的用户名\n");
	CLOSE
	printf("屏蔽好友输入             ");
	RED
	printf("shield:加好友名字\n");
	CLOSE
	printf("创建群聊输入             ");
	RED
	printf("create即可\n");
	CLOSE
	printf("删除群聊输入             ");
	RED
	printf("rm_group:加群名即可\n");
	CLOSE
	printf("申请入群请输入           ");
	RED
	printf("add_g:加群名\n");
	CLOSE
	printf("向群发送消息输入         ");
	RED
	printf("g:加群名:消息即可\n");
	CLOSE
	printf("查看群消息历史记录输入   ");
	RED
	printf("g_his:加群名即可\n");
	CLOSE
	printf("查看所加入的群聊输入     ");
	RED
	printf("w即可\n");
	CLOSE
	printf("查看群成员输入           ");
	RED
	printf("w:加群名即可\n");
	CLOSE
	printf("设置管理员输入           ");
	RED
	printf("set:加群名:加用户名\n");
	CLOSE
	printf("=================================================\n");
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, send_message, (void *)&cli_fd);
	pthread_create(&tid2, NULL, recv_message, (void *)&cli_fd);
	pthread_join(tid1, (void *)&status);
	pthread_join(tid2, (void *)&status);
}

