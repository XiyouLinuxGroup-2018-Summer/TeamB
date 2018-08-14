#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#define	PORT	8888				//服务器端口地址
void process_conn_client(int s);
int main(int argc,char *argv[])
{
	int s;								//socket描述符
	struct sockaddr_in serv_addr;		//服务器地址结构
	if((s = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		printf("socket error\n");
		return -1;
	}

	//设置服务器地址
	bzero(&serv_addr,sizeof(serv_addr));			//清零
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//IP地址
	serv_addr.sin_port = htons(PORT);				//服务器端口
	inet_pton(AF_INET,argv[1],&serv_addr.sin_addr);	//将用户输入的字符类型的IP地址转为整形

	connect(s,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr));
	while(1) {
		process_conn_client(s);
		close(s);
	}
	return 0;
}
void process_conn_client(int s)
{
	ssize_t size = 0;
	char buffer[1024];
		gets(buffer);
			write(s,buffer,size);						//发送给服务器
}
