#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#define PORT_SERV 8888
#define BUFF_LEN 256
void udpclie_echo(int s,struct sockaddr *to)
{
	char buff[BUFF_LEN];
	struct sockaddr_in from;										
	socklen_t len = sizeof(*to);									
	sendto(s,buff,BUFF_LEN,0,to,len);								//发送给服务器
	recvfrom(s,buff,BUFF_LEN,0,(struct sockaddr *)&from,&len);		//从服务器接收数据
	printf("recved:%s\n",buff);
}
int main(int argc,char *argv[])
{	
	int s;															//文件描述符
	struct sockaddr_in addr_serv;									//地址结构
	s = socket(AF_INET,SOCK_DGRAM,0);								//建立UDP类型套接字
	memset(&addr_serv,0,sizeof(addr_serv));							//清空地址结构
	addr_serv.sin_family = AF_INET;
	addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);					
	addr_serv.sin_port = htons(PORT_SERV);							//服务器端口

	udpclie_echo(s,(struct sockaddr *)&addr_serv);					//与服务器交互
	close(s);
	return 0;
}
