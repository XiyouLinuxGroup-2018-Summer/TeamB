#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#define PORT 8888					//服务器端口地址
#define BACKLOG	12					//最大连接队列长度

void process_conn_server(int s);
int main(void)
{
	char buffer[1024];
	int ss,sc;						//ss为服务器的socket描述符,sc为客户端的socket描述符
	struct sockaddr_in	serv_addr;	//服务器地址结构
	struct sockaddr_in	cli_addr;	//客户端地址结构
	int err;						//返回值
	pid_t pid;						

	//建立一个AF_INET域的TCP类型的套接字
	if((ss = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		printf("socket error\n");
		return -1;
	}

	//设置服务器地址
	bzero(&serv_addr,sizeof(serv_addr));			//清零
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//IP地址
	serv_addr.sin_port = htons(PORT);				//服务器端口
	
	//绑定地址到套接字描述符
	if((err = bind(ss,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)) {
		printf("bind error\n");
		return -1;
	}

	//设置监听队列
	if((err = listen(ss,BACKLOG)) < 0) {
		printf("listen error\n");
		return -1;
	}

	while(1) {
		socklen_t addrlen = sizeof(struct sockaddr);
		if((sc = accept(ss,(struct sockaddr *)&serv_addr,&addrlen)) < 0)
			continue;

		//建立一个新的进程处理到来的连接
		pid = fork();
		if(pid == 0) {
			printf("connect successful!\n");
			read(sc,buffer,1024);
			printf("%s\n",buffer);
			//process_conn_server(sc);
			close(ss);
			exit(1);
		}
		else
			close(sc);
		
	}
	return 0;
}

//服务器对客户端的处理
void process_conn_server(int s)
{
	ssize_t size = 0;
	char buffer[1024];
	
		size = read(s,buffer,1024);
		if(size == 0)
			return;
		printf("%s\n",buffer);	
}



