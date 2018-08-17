
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include "myQQ.h"
#define PORT		5678

char *user;							//记录此时登录的用户
b_data back_data;
int count;
pthread_mutex_t mutex;
char *passwd;
char *friend;						//记录当前正在聊天的好友


//创建接受服务器反馈结果的线程
void *fback(void *arg)
{
	int conn_fd = *(int *)arg;	
	int num = 0;
	while(1) {
		if((num += recv(conn_fd,&back_data,sizeof(back_data),0)) < 0) {
			perror("recv");
		}
		if(num == sizeof(back_data)) {						//确保接收到完整的结构体
			count = 1;
			num = 0;
			if(back_data.type == 431)						//是接收到的消息
				printf("用户%s请求加您为好友",back_data.ar[0].send_user);
			else if(back_data.type == 41) {
				if(strcmp(user,back_data.ar[0].recv_user) == 0 && strcmp(friend,back_data.ar[0].send_user) == 0)
					printf("%s:%-50s\n",friend,back_data.ar[0].data);
				else
					printf("您有一条来自好友%s的新消息，请及时查看\n",back_data.ar[0].send_user);
			}
		}
	}
}


int main(int argc,char *argv[])
{
	struct sockaddr_in serv_addr;
	int sock_fd;
	request buf;
	sock_fd = socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd < 0) {
		perror("socket:");
	}

	//初始化服务器端口与地址
	memset(&serv_addr,0,sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);	//服务器端口
	inet_pton(AF_INET,argv[1],&serv_addr.sin_addr);	
	
	//连接服务器
	connect(sock_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr));
	if(sock_fd < 0) {
		perror("connect:");
	}
	pthread_t thid;
	pthread_create(&thid,NULL,fback,(void *)&sock_fd);
	
	//选择功能
	printf("1.登录\n2.注册\n3.退出\n");
	char choice;
	while(scanf("%c",&choice) && choice != '3') {
		switch(choice) {
			case '1':
				buf.type = 0100;
				printf("请输入用户名:");
				scanf("%s",buf.send_user);
				user = buf.send_user;
				printf("请输入密码:");
				scanf("%s",buf.data);
				passwd = buf.data;					//使用全局变量保存登陆者的信息以便在其他函数中调用
				buf.flag = 0;
				
				memset(&back_data,0,sizeof(back_data));
				count = 0;							//接收标志置为0
				send(sock_fd,&buf,sizeof(buf),0);	//发送请求
			
				while(1) {
					if(count == 1 && back_data.cnt == 1) {
						user = buf.send_user;
						puts("登陆成功!");
						Main_menu(sock_fd);								//进入管理界面
						break;
					}
					else if(count == 1 && back_data.cnt == 0) {
						puts("密码错误");
						break;
					}
				}
				break;
			case '2':
				buf.type = 0110;
				printf("请输入新的用户名:");
				scanf("%s",buf.send_user);
				printf("请输入新的密码:");
				scanf("%s",buf.data);
				buf.fd = sock_fd;
				buf.flag = 0;
				
				memset(&back_data,0,sizeof(back_data));
				count= 0;							//接收标志置为0
				send(sock_fd,&buf,sizeof(buf),0);	//发送请求
				
				while(1) {
					if(count == 1 && back_data.cnt == 1) {
						printf("注册成功");
						break;
					}
					else if(count == 1 && back_data.cnt == 0)
						puts("注册失败");
				}
				break;
			default:
				break;
		}
		while(getchar() != '\n');
		printf("1.登录\n2.注册\n3.退出\n");
	}
	return 0;
}

//管理界面，包括好友管理，群聊管理，消息管理，账号管理
void Main_menu(int fd)
{
	int choice;
	request buf;
	printf("1.好友管理\n2.群管理\n3.消息管理\n4.账号管理\nq.返回上一层\n");
	while(scanf("%d",&choice) && choice != 'q') {
		switch(choice) {
			case 1:
				Friend_Manage(fd);					//好友管理
				break;
			case 2:
	//			Group_Manage(fd);					//群管理
				break;
			case 3:
	//			News_Manage(fd);					//消息管理
				break;
			case 4:
	//			Account_Manage(fd);					//账号管理
				break;
			default:
				printf("请输入正确的选项\n");
		}
		printf("1.好友管理\n2.群管理\n3.消息管理\n4.账号管理\nq.返回上一层\n");
		while(getchar() != '\n');
	}
	return;
}

//好友管理界面
void Friend_Manage(int fd)
{
	//显示所有好友
	request buf;
	buf.type = 0240;
	strcpy(buf.send_user,user);
	buf.fd = fd;
	buf.flag = 0;
	int i;

	count = 0;
	memset(&back_data,0,sizeof(back_data));
	send(fd,&buf,sizeof(buf),0);					//发送请求
	while(1) {
		if(count == 1 && back_data.str[0][0] != '\0') {
			for(i = 0;back_data.str[i][0] != '\0';i++)
				printf("%s\n",back_data.str[i]);
			break;
		}
		else if(count == 1 && back_data.str[0][0] == '\0') {
			printf("您目前还没有好友\n");
			break;
		}
	}

	printf("1.私聊\n2.添加好友\n3.删除好友\n4.查询好友\n5.查看聊天记录\nq.退出\n");
	int choice;
	int choice2;
	char friend_name[20];
	memset(&buf,0,sizeof(buf));
	while(scanf("%d",&choice) && choice != 'q') {
		switch(choice) {
			case 1:
				printf("请输入好友昵称:");
				scanf("%s",buf.recv_user);
				friend = buf.recv_user;
				strcpy(friend_name,buf.recv_user);
				buf.type = 0250;
				strcpy(buf.send_user,user);
				buf.fd = fd;
				buf.flag = 0;

				memset(&back_data,0,sizeof(back_data));	//清空接收结构体
				count = 0;								//将接收标志置为0
				send(fd,&buf,sizeof(buf),0);			//发送查看聊天记录请求

				//显示之前五十条聊天记录
				while(1) {
					if(count == 1 && back_data.ar[0].data[0] != '\0') {
						for(i = 0;back_data.ar[i].data[0] != '\0';i++)
							if(strcmp(back_data.ar[i].send_user,user) == 0)
								printf("%50s\n",back_data.ar[i].data);
							else
								printf("%-50s\n",back_data.ar[i].data);
						break;
					}
					else if(count == 1 && back_data.ar[0].data[0] == '\0')
						break;
				}
				printf("\nhello\n");

				//发送消息
				printf("1.发送消息\nq.退出\n");
				while(scanf("%d",&choice2) && choice2 != 'q') {
					switch(choice2) {
						case 1:
							memset(&buf,0,sizeof(buf));		//清空请求结构体
							printf("请输入你想发送的内容:");
							scanf("%s",buf.data);
							buf.type = 0200;
							strcpy(buf.send_user,user);
							buf.fd = fd;
							strcpy(buf.recv_user,friend_name);
							int flag = 0;

							count = 0;					//将接收标志置0
							memset(&back_data,0,sizeof(back_data));	//清空接收结构体
							send(fd,&buf,sizeof(buf),0);			//发送消息请求
							
							break;
						default:
							printf("请输入正确的选项\n");
							break;
					}
					printf("1.发送消息\nq.退出\n");
				}
				break;
			case 2:
				//添加好友
				memset(friend_name,0,20);
				memset(&buf,0,sizeof(buf));
				memset(&back_data,0,sizeof(b_data));
				
				printf("请输入好友昵称:");
				scanf("%s",friend_name);
				printf("请输入验证信息:");
				scanf("%s",buf.data);
				
				buf.type = 0220;
				strcpy(buf.send_user,user);
				strcpy(buf.recv_user,friend_name);
				buf.fd = fd;
				buf.flag = 0;

				count = 0;
				memset(&back_data,0,sizeof(back_data));
				send(fd,&buf,sizeof(buf),0);
				while(1) {
					if(count == 1 && back_data.cnt == 1) { 
						printf("请求发送成功!\n");
						break;
					}
					else if(count == 1 && back_data.cnt  == 0) {
						printf("添加失败!\n");
						break;
					}
				}
				break;
			case 3:
				//删除好友
				memset(friend_name,0,20);
				memset(&buf,0,sizeof(buf));
				memset(&back_data,0,sizeof(b_data));

				printf("请输入欲删除好友昵称:");
				scanf("%s",buf.recv_user);

				buf.type = 0220;
				strcpy(buf.send_user,user);
				buf.fd = fd;
				buf.flag = 0;

				count = 0;
				memset(&back_data,0,sizeof(back_data));
				send(fd,&buf,sizeof(buf),0);
				while(1) {
					if(count == 1 && back_data.cnt == 1) {
						printf("删除成功");
						break;
					}
					else if(count == 1 && back_data.cnt == 0) {
						printf("删除失败");
						break;
					}
				}
				break;
			case 4:
				break;
			case 5:
				break;
			default:
				printf("请输入正确选项");
		}
		printf("1.私聊\n2.添加好友\n3.删除好友\n4.查询好友\n5.查看聊天记录\nq.退出\n");
	}
}



