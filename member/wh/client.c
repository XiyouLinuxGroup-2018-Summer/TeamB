#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
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
char group[20];						//记录当前正在聊天的群聊	
char friend[20];					//记录当前正在聊天的好友
struct record cr[50];				//存放所有被动接收到的消息
int cnt = 0;						//存放被动接收到的消息的数量


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
			
			if(back_data.type == 431)	{					//是接收到的消息
				printf("用户%s请求加您为好友\n",back_data.ar[0].send_user);
				memcpy((void *)&cr[cnt++],(void *)&back_data.ar[0],sizeof(struct record));
			}
			else if(back_data.type == 41) {					//正在和此好友聊天
				if(strcmp(user,back_data.ar[0].recv_user) == 0 && strcmp(friend,back_data.ar[0].send_user) == 0) {
					printf("%s:%-50s\n",friend,back_data.ar[0].data);
					back_data.flag = 1;
				}
				else {										//没有和此好友聊天
					printf("您有一条来自好友%s的新消息，请及时查看\n",back_data.ar[0].send_user);
					memcpy((void *)&cr[cnt++],(void *)&back_data.ar[0],sizeof(struct record));
				}
			}
			else if(back_data.type == 42) {					//正在水群
				if(strcmp(user,back_data.ar[0].recv_user) == 0 && strcmp(group,back_data.ar[0].send_user) == 0) {	
					printf("%s:%-50s\n",back_data.ar[0].send_user,back_data.ar[0].data);
					back_data.flag = 1;
				}
				else {										//没有水群
					printf("您有一条来自群聊%s的新消息，请及时查看\n",back_data.ar[0].send_user);
					memcpy((void *)&cr[cnt++],(void *)&back_data.ar[0],sizeof(struct record));
				}
			}
			count = 1;					//经过处理之后将是否接收到该消息标志置为1
			num = 0;
			
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
				memset(&buf,0,sizeof(buf));
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
				Group_Manage(fd);					//群管理
				break;
			case 3:
				News_Manage(fd);					//消息管理
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
				//显示之前五十条聊天记录
				memset(friend,0,20);
				strcpy(friend,buf.recv_user);
				strcpy(friend_name,buf.recv_user);
				buf.type = 0250;
				strcpy(buf.send_user,user);
				buf.fd = fd;
				buf.flag = 0;
				

				memset(&back_data,0,sizeof(back_data));	//清空接收结构体
				count = 0;								//将接收标志置为0
				send(fd,&buf,sizeof(buf),0);			//发送查看聊天记录请求

				while(1) {
					if(count == 1 && back_data.ar[0].data[0] != '\0') {
						for(i = 0;back_data.ar[i].data[0] != '\0';i++) {
							if(strcmp(back_data.ar[i].send_user,user) == 0)
								printf("%50s:%s\n",back_data.ar[i].data,back_data.ar[i].recv_user);
							else
								printf("%s:%-50s\n",user,back_data.ar[i].data);
						}
						break;
					}
					else if(count == 1 && back_data.ar[0].data[0] == '\0')
						break;
				}

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
							buf.time = time(NULL);
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
				
				buf.time = time(NULL);
				buf.type = 0210;
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

//消息管理界面
void News_Manage(int fd)
{
	int i;
	char choice;
	char choice2;
	char choice3;
	char choice4;
	char friend_name[20];					//存放好友姓名的字符串数组
	request buf;

	memset(friend_name,0,20);
	while(getchar() != '\n');
	printf("1.私聊\n2.群聊\n3.系统通知\nq.退出\n");
	while(scanf("%c",&choice) && choice != 'q') {
		switch(choice) {
			case '1':
				
				for(i = 0;i < cnt;i++) {
					if(cr[i].type == 1) 
						printf("%s %s\n",cr[i].send_user,asctime((localtime(&cr[i].time))));
				}
				printf("1.我要回复\nq.算了懒得回了(退出)\n");
				//进入私聊界面
				while(scanf("%c",&choice3) && choice3 != 'q') {
					switch(choice3) {
						case '1':
							Chat_Friend(fd);
							break;
					}
					while(getchar() != '\n');
					printf("1.我要回复\nq.算了懒得回了(退出)\n");
				}
				break;

			case '2':
				for(i = 0;i < cnt;i++) {
					if(cr[i].type == 2) 
						printf("%s %s\n",cr[i].send_user,asctime((localtime(&cr[i].time))));
				}
				
				
				break;
			
			case '3':
				//显示所有系统通知和好友请求
				for(i = cnt-1;i >= 0;i--) {
					if(cr[i].type == 3) 
						printf("%s %s\n",cr[i].send_user,asctime((localtime(&cr[i].time))));
						printf("%s\n",cr[i].data);
				}
	//			printf("1.我要处理\nq.算了就当没看见(退出)\n");
	//			for(i = 0;i < cnt;i++) {
	//				if(ar[i].type == 31)
				break;
					
			default:

				printf("请输入正确的选项\n");
				break;
		}
		while(getchar() != '\n');
		printf("1.私聊\n2.群聊\n3.系统通知\nq.退出\n");
	}
}

//与好友私聊界面
void Chat_Friend(int fd)
{
	request buf;
	char friend_name[20];
	int i;
	char choice2;
	
	memset(friend_name,0,20);
	printf("请输入好友昵称:");
	scanf("%s",buf.recv_user);
	//显示之前五十条聊天记录
	memset(friend,0,20);
	strcpy(friend,buf.recv_user);
	printf("%s\n",friend);
	strcpy(friend_name,buf.recv_user);
	buf.type = 0250;
	strcpy(buf.send_user,user);
	buf.fd = fd;
	buf.flag = 0;


	memset(&back_data,0,sizeof(back_data));	//清空接收结构体
	count = 0;								//将接收标志置为0
	send(fd,&buf,sizeof(buf),0);			//发送查看聊天记录请求

	while(1) {
		if(count == 1 && back_data.ar[0].data[0] != '\0') {
			for(i = 0;back_data.ar[i].data[0] != '\0';i++) {
				if(strcmp(back_data.ar[i].send_user,user) == 0)
					printf("%50s:%s\n",back_data.ar[i].data,back_data.ar[i].recv_user);
				else
					printf("%s:%-50s\n",user,back_data.ar[i].data);
			}
			break;
		}
		else if(count == 1 && back_data.ar[0].data[0] == '\0')
			break;
	}

	//发送消息
	printf("1.发送消息\nq.退出\n");
	while(getchar() != '\n');
	while(scanf("%c",&choice2) && choice2 != 'q') {
		switch(choice2) {
			case 1:
				memset(&buf,0,sizeof(buf));		//清空请求结构体
				printf("请输入你想发送的内容:");
				scanf("%s",buf.data);
				buf.type = 0200;
				strcpy(buf.send_user,user);
				buf.fd = fd;
				strcpy(buf.recv_user,friend_name);
				buf.time = time(NULL);
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
}

//群聊界面
void Chat_Group(int fd)
{
	request buf;
	int i;
	char choice2;
	memset(&buf,0,sizeof(buf));
	printf("请输入群聊名称:");
	scanf("%s",buf.recv_user);
	memset(group,0,20);
	strcpy(group,buf.recv_user);

	//显示出群聊最近的聊天记录
	//初始化请求
	buf.type = 0370;
	strcpy(buf.send_user,user);
	buf.flag = 0;

	//发送请求
	count = 0;
	memset(&back_data,0,sizeof(b_data));
	send(fd,&buf,sizeof(buf),0);

	//显示该群聊天记录
	while(1) {
		if(count == 1 && back_data.ar[0].data[0] != '\0') {
			for(i = 0;back_data.ar[i].data[0] != '\0';i++) {
				if(strcmp(user,back_data.ar[i].send_user) == 0)
					printf("%50s:%s\n",user,back_data.ar[i].data);
				else
					printf("%s:%s\n",back_data.ar[i].send_user,back_data.ar[i].data);
			}
			break;
		}
		else if(count == 1 && back_data.ar[0].data[0] == '\0')
			break;
	}


	//发送消息
	printf("1.发送消息\nq.退出\n");
	
	while(scanf("%c",&choice2) && choice2 != 'q') {
		while(getchar() != '\n');
		switch(choice2) {
			case 1:
				memset(&buf,0,sizeof(buf));		//清空请求结构体
				printf("请输入你想发送的内容:");
				scanf("%s",buf.data);
				buf.type = 0300;					//请求类型为群聊请求
				strcpy(buf.send_user,user);
				buf.fd = fd;
				strcpy(buf.recv_user,group);
				buf.time = time(NULL);
				int flag = 0;

				count = 0;					//将接收标志置0
				memset(&back_data,0,sizeof(back_data));	//清空接收结构体
				send(fd,&buf,sizeof(buf),0);			//发送消息

				break;
			default:
				printf("请输入正确的选项\n");
				break;
		}
		printf("1.发送消息\nq.退出\n");
	}


}
//群聊管理界面
void Group_Manage(int fd)
{
	request buf;
	char choice;
	int i;
	char create_group[20];

	printf("1.群聊\n2.创建群\n3.解散群\n4.加入群\nq.退出");
	while(scanf("%c",&choice) && choice != 'q') {
		while(getchar() != '\n');			//清空缓冲区
		switch(choice) {
			//群聊
			case 1:
				Chat_Group(fd);
				break;
			//创建群
			case 2:
				printf("请输入要创建的群名称:");
				memset(create_group,0,20);
				scanf("%s",create_group);

				//初始化请求
				strcpy(buf.data,create_group);
				strcpy(buf.send_user,user);
				buf.type = 0310;

				//发送请求
				count = 0;
				memset(back_data,0,sizeof(b_data));
				send(fd,&buf,sizeof(buf),0);
			
				while(1) {
					if(count == 1 && back_data.cnt == 1) {
						printf("您的群聊%s创建成功\n",create_group);
						break;
					}
					else if(count == 1 && back_data.cnt == 0) {
						printf("创建失败\n");
						break;
					}
				}
				break;
			default:
				printf("请输入正确的选项\n");
				break;
		}
		printf("1.群聊\n2.创建群\n3.解散群\n4.加入群\nq.退出");
	}
}






