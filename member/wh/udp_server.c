#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#define BUFF_LEN 256
void udpserv_echo(int s,struct sockaddr *client)
{
	int n;								//接收数据长度
	char buff[BUFF_LEN];				//接收发送缓冲区
	socklen_t	len;					//地址长度
	while(1) {
		len = sizeof(*client);
		n = recvfrom(s,buff,BUFF_LEN,0,client,&len);
		sendto(s,buff,n,0,client,len);	//将接收到的字符发送回客户端
	}
}
int main(void)					
{
	int s;								//套接字文件描述符
	struct sockaddr_in	local;			//本地的地址信息
	struct sockaddr_in	from;			//发送方的地址信息
	int from_len = sizeof(from);		//地址结构的长度
	int n;								//接收到的数据长度
	char buf[128];						//接受数据缓冲区
	s= socket(AF_INET,SOCK_DGRAM,0);	//初始化一个IPV4族的数据报套接字
	if(-1 == s) {
		perror("");
		exit(EXIT_FAILURE);
	}

	local.sin_family = AF_INET;
	local.sin_port = htons(8888);
	local.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(s,(struct sockaddr *)&local,sizeof(local));
	udpserv_echo(s,(struct sockaddr *)&from);	//回显处理函数
	return 0;
}
	

