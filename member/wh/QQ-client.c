#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#define MAXLINE			1024
#define IPADDRESS		"127.0.0.1"
#define SERV_PORT		4567

#define max(a,b) (a > b) ? a : b

//用户信息结构体
typedef struct use{
	char name[20];					//用户名
	char passwd[20];				//密码
	int fd;							//用户此次登录时所使用的套接字	
}user;

//消息信息结构体
typedef struct new{
	char *send_user;				//发送方
	char *recv_user;				//接收方
	char data[50];					//发送的消息
}news;

int main(int argc,char *argv[])
{
	int sockfd;
	struct sockaddr_in servaddr;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,IPADDRESS,&servaddr.sin_addr);
	connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	handle_connection(sockfd);
	return 0;
}

void handle_connextion(int sockfd)
{
	char sendline[MAXLINE],recvline[MAXLINE];
	int maxfdp,stdineof;
	struct pollfd pfds[2];
	int n;
	//添加连接描述符
	pfds[0].fd = sockfd;
	pfds[0].events = POLLIN;
	//添加标准输入描述符
	pfds[1].fd = STDIN_FILENO;
	pfds[1].events = POLLIN;
	while(1) {
		poll(pfds,2,-1);
		if(pfds[0].revents & POLLIN) {
			n = read(


































