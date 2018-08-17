/*2018-8-13 周一 未完成
实现最简单的两个客户端进程直接相互通信，服务器端负责接受用户发送的消息并发送到与之进行通信的用户
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <sys/types.h>

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

#define IPADDRESS					"127.0.0.1"
#define PORT						"4567"
#define LISTENQ						5
#define OPEN_MAX					1000
#define INFTIM						-1

int main(void)
{
	//创建套接字并和端口绑定
	struct sockaddr_in	serv_addr;
	int listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd == -1) {
		perror("socket error:");
		exit(1);
	}
	bzero(&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	inet_pton(AF_INET,IPADDRESS.&serv_addr,sin_addr);
	serv_addr.sin_port = htons(PORT);
	if(bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1) {
		perror("bind error:");
		exit(1);
	}
	listen(listenfd,LISTENQ);

	do_poll(listenfd);
	return 0;
}

void do_poll(int listenfd)
{
	int connfd,sockfd;
	struct sockaddr_in cliaddr;
	socklen_t cliaddrlen;
	struct pollfd clientfds[OPEN_MAX];
	int maxi;
	int i;
	int nready;
	//添加监听描述符
	cliendfds[0].fd = listenfd;
	clientfds[0].events = POLLIN;
	//初始化客户连接描述符
	for(i = 0;i < OPEN_MAX;i++)
		clientfds[i].fd = -1;
	maxi = 0;
	//循环处理
	while(1) {
		//获取可用描述符的个数
		nready = poll(clientfds,maxi+1,INFTIM);
		if(nready == -1) {
			perror("poll error:");
			exit(1);
		}
		//测试监听描述符是否准备好
		if(clientfds[0].revents & POLLIN) {
			cliaddrlen = sizeof(cliaddr);
			//接受新的连接
			if((connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&cliaddrlen)) == -1) {
				if(error == EINTR)
					continue;
				else {
					perror("accept error:");
					exit(1);
				}
			}
			fprintf(stdout,"accept a new client: %s:%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);
			//将新的连接描述符添加到数组中
			for(i = 1;i < OPEN_MAX;i++) {
				if(clientfds[i].fd < 0) {
					clientfds[i].fd = connfd;
					break;
				}
			}
			if(i == OPEN_MAX) {
				fprintf(stderr,"too many clients.\n");
				exxit(1);
			}
			//将新的描述符添加到读描述符集合中
			clientfds[i].events = POLLIN;
			//记录客户连接套接字个数
			maxi = (i > maxi)?i:max;
			if(--nready <= 0)
				continue;
		}
		//处理客户连接
		handle_connection(clientfds,maxi);
	}
}

void handle_connection(struct pollfd *connfds,int num)
{
	int i,n;
	news buf;
	memset(buf,0,sizeof(buf));
	for(i = 1;i <= num;i++) {
		if(connfds[i].fd < 0)
			continue;
		//测试客户描述符是否准备好
		if(connfds[i].revents & POLLIN) {
			//接受客户端发送的信息
			n = read(connfds[i].fd,buf,sizeof(buf));
			if(n == 0) {
				close(connfds[i].fd);
				connfds[i].fd = -1;
				continue;
			}
			//向buf中所存储的接收方发送data
			write(buf.recv_user,buf.data,n);
		}
	}
}
