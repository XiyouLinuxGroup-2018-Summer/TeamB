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
#include <sys/stat.h>
#include <fcntl.h>
#include "myQQ.h"
#define PORT		5678

char user[20];							//记录此时登录的用户
b_data back_data;
int count;
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
	int length = 0;
	while(1) {
		if((length= recv(conn_fd,((char *)&back_data + num),sizeof(back_data) - num ,0)) < 0) {
			perror("recv");
		}
		num+=length;
		if(num == sizeof(b_data)) {						//确保接收到完整的结构体
			if(back_data.type == 431)	{					//接收到好友申请
				if(back_data.flag == 0)	{					//提交的申请
					printf("用户%s请求加您为好友\n",back_data.ar[0].send_user);
					back_data.ar[0].flag = 0;
					back_data.ar[0].type = 31;
					memcpy((void *)&cr[cnt++],(void *)&back_data.ar[0],sizeof(struct record));
				}
				else {										//已同意的反馈
					printf("用户%s同意添加您为好友\n",back_data.ar[0].recv_user);
				}
			}
			else if(back_data.type == 432) {				//接收到请求加群的通知
				if(back_data.flag == 0) {
					printf("用户%s请求加入群聊%s\n",back_data.ar[0].send_user,back_data.ar[0].data);
					back_data.ar[0].flag = 0;
					back_data.ar[0].type = 32;
					memcpy((void *)&cr[cnt++],(void *)&back_data.ar[0],sizeof(struct record));
				}
				else										//接收到同意加群的反馈
					printf("您成功加入群聊%s\n",back_data.ar[0].data);
			}
			else if(back_data.type == 433) {				//接收到解散群的通知
				printf("群主%s已解散群聊%s\n",back_data.ar[0].send_user,back_data.ar[0].recv_user);
				memcpy((void *)&cr[cnt++],(void *)&back_data.ar[0],sizeof(struct record));
			}
			else if(back_data.type == 434) {				//接收到用户退出的通知
				printf("用户%s已退出群聊%s\n",back_data.ar[0].send_user,back_data.ar[0].recv_user);
				memcpy((void *)&cr[cnt++],(void *)&back_data.ar[0],sizeof(struct record));
			}
			else if(back_data.type == 41) {					//正在和此好友聊天
				if(strcmp(friend,back_data.ar[0].send_user) == 0) {
					printf("%s:%-50s\n",friend,back_data.ar[0].data);
					back_data.flag = 1;
				}
				else {										//没有和此好友聊天
					printf("您有一条来自好友%s的新消息，请及时查看\n",back_data.ar[0].send_user);
					back_data.ar[0].type = 1; 
					memcpy((void *)&cr[cnt++],(void *)&back_data.ar[0],sizeof(struct record));
				}
			}
			else if(back_data.type == 42) {					//正在水群
				if(strcmp(group,back_data.ar[0].recv_user) == 0) {	
					printf("%s:%-50s\n",back_data.ar[0].send_user,back_data.ar[0].data);
					back_data.flag = 1;
				}
				else {										//没有水群
					printf("您有一条来自群聊%s的新消息，请及时查看\n",back_data.ar[0].recv_user);
					memcpy((void *)&cr[cnt++],(void *)&back_data.ar[0],sizeof(struct record));
				}
			}
			else if(back_data.type == 44 && back_data.flag == 0) {				//准备好了接收文件
				printf("您有一条来自用户%s的请求，请及时查看\n",back_data.ar[0].send_user);
				memcpy((void *)&cr[cnt++],(void *)&back_data.ar[0],sizeof(struct record));

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
	printf("1.登录\n2.注册\nq.退出\n");
	char choice;
	memset(user,0,20);
	while(scanf("%c",&choice) && choice != 'q') {
		system("clear");
		switch(choice) {
			case '1':
				memset(&buf,0,sizeof(buf));
				buf.type = 0100;
				printf("请输入用户名:");
				scanf("%s",buf.send_user);
				strcpy(user,buf.send_user);
				printf("请输入密码:");
				scanf("%s",buf.data);
				passwd = buf.data;					//使用全局变量保存登陆者的信息以便在其他函数中调用
				buf.flag = 0;
				
				memset(&back_data,0,sizeof(back_data));
				count = 0;							//接收标志置为0
				send(sock_fd,&buf,sizeof(buf),0);	//发送请求
			
				while(1) {
					if(count == 1 && back_data.cnt == 1) {
						puts("登陆成功!");
						
						//发送显示离线消息请求
						memset(&buf,0,sizeof(buf));
						buf.type = 0500;
						strcpy(buf.send_user,user);
						send(sock_fd,&buf,sizeof(buf),0);
						
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
						printf("注册成功\n");
						break;
					}
					else if(count == 1 && back_data.cnt == 0) {
						puts("注册失败,该用户已存在\n");
						break;
					}
				}
				break;
			default:
				break;
		}
		while(getchar() != '\n');
		printf("1.登录\n2.注册\nq.退出\n");
	}
	system("clear");
	
	return 0;
}

//管理界面，包括好友管理，群聊管理，消息管理，账号管理
void Main_menu(int fd)
{
	int choice;
	request buf;
	printf("1.好友管理\n2.群管理\n3.消息管理\nq.注销登录\n");
	while(scanf("%d",&choice) && choice != 'q') {
		system("clear");
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
		printf("1.好友管理\n2.群管理\n3.消息管理\nq.注销登录\n");
		while(getchar() != '\n');
	}
	system("clear");
	
	//处理注销请求
	
	memset(&buf,0,sizeof(buf));
	buf.type = 0130;
	strcpy(buf.send_user,user);	
	send(fd,&buf,sizeof(buf),0);
	memset(user,0,20);

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
		system("clear");
		switch(choice) {
			case 1:
				printf("请输入好友昵称:");
				scanf("%s",buf.recv_user);
				//显示之前五十条聊天记录
				memset(friend,0,20);
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
								printf("%50s:%s\n",back_data.ar[i].data,user);
							else
								printf("%s:%-50s\n",friend_name,back_data.ar[i].data);
						}
						break;
					}
					else if(count == 1 && back_data.ar[0].data[0] == '\0')
						break;
					else if(count == 1 && back_data.cnt == 0) {
						break;
					}

				}
				if(back_data.cnt == 0) {
					printf("您与该用户不是好友关系\n");
					break;
				}

				//发送消息
				printf("1.发送消息\n2.发送文件\nq.退出\n");
				strcpy(friend,friend_name);			//设置正在聊天的好友
				while(getchar() != '\n');
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
						case 2: { 
							memset(&buf,0,sizeof(buf));
							printf("输入文件名:");
							scanf("%s",buf.data);
							//打开当前目录下的此文件，计算其大小，将文件名连同大小发送到服务器
							int filed;
							struct stat auf;
							char c;									//用一字节来传输文件
							if((filed = open(buf.data,O_RDWR) )== -1) 
								perror("");

							if(lstat(buf.data,&auf) == -1) 
								perror("");

							//初始化请求结构体
							buf.type = 0400;
							strcpy(buf.recv_user,friend_name);
							strcpy(buf.send_user,user);
							buf.flag = 0;
							buf.size = auf.st_size;
							int num = 0;
							
							count = 0;
							memset(&back_data,0,sizeof(back_data));
							send(fd,&buf,sizeof(buf),0);
							printf("正在等待对方同意\n");
							while(1) {
								printf("#");
								sleep(1);
								if(count == 1 && back_data.cnt == 1) {
									//开始传输文件
									printf("*");
									memset(&buf,0,sizeof(buf));
									buf.type = 0400;
									buf.flag = 1;
									strcpy(buf.recv_user,friend_name);
									strcpy(buf.send_user,user);
									if((num += read(filed,buf.data,50)) == auf.st_size) {//到末尾结束
																	//清空请求数据以此作为结束标志
										memset(buf.data,0,50);
										strcpy(buf.data,"wanbi");
										send(fd,&buf,sizeof(buf),0);
										printf("发送完毕\n");
										break;
									}
									send(fd,&buf,sizeof(buf),0);
									count = 0;
								}

							}
							break;	
									

						}
						default:
							printf("请输入正确的选项\n");
							break;
					}
					system("clear");
					/*
					//显示聊天记录
					strcpy(buf.send_user,friend_name);
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
									printf("%50s:%s\n",back_data.ar[i].data,back_data.ar[i].send_user);
								else
									printf("%s:%-50s\n",back_data.ar[i].recv_user,back_data.ar[i].data);
							}
							break;
						}
						else if(count == 1 && back_data.ar[0].data[0] == '\0')
							break;
					}
					*/
	
					printf("1.发送消息\n2.发送文件\nq.退出\n");
				}
				memset(friend,0,20);				//将正在聊天的好友置0
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
						printf("删除成功\n");
						break;
					}
					else if(count == 1 && back_data.cnt == 0) {
						printf("删除失败\n");
						break;
					}
				}
				break;
			case 4:
				break;
			case 5:
				break;
			default:
				printf("请输入正确选项\n");
		}
		printf("1.私聊\n2.添加好友\n3.删除好友\n4.查询好友\n5.查看聊天记录\nq.退出\n");
	}
	system("clear");
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
	printf("1.私聊\n2.群聊\n3.系统通知\n4.文件传输\nq.退出\n");
	while(scanf("%c",&choice) && choice != 'q') {
		system("clear");
		switch(choice) {
			case '1':
				
				for(i = 0;i < cnt;i++) {
					if(cr[i].type == 1) 
						printf("%s %s\n",cr[i].send_user,asctime((localtime(&cr[i].time))));
				}
				printf("1.我要回复\nq.算了懒得回了(退出)\n");
				//进入私聊界面
				while(getchar() != '\n');
				while(scanf("%c",&choice3) && choice3 != 'q') {
					system("clear");
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
				//处理所有系统通知和好友请求
				for(i = 0;i < cnt;i++) {
					if(cr[i].type == 31 && cr[i].flag == 0) {
						printf("%s请求加您为好友:(y/n)(第%d条，还有%d条待处理)\n验证消息%s\n",cr[i].send_user,i+1,cnt-i-1,cr[i].data);
						char c;
						while(getchar() != '\n');
						scanf("%c",&c);
						if(c == 'y') {
							memset(&buf,0,sizeof(buf));
							buf.type = 0210;
							//处理申请后发送同意添加请求给服务器
							strcpy(buf.send_user,cr[i].send_user);
							strcpy(buf.recv_user,cr[i].recv_user);
							
							buf.flag = 1;
							
							//初始化反馈结构体和接收反馈标志
							memset(&back_data,0,sizeof(b_data));
							if(send(fd,&buf,sizeof(buf),0) == sizeof(buf))
								printf("已添加\n");

							//无接收反馈,故接收标志不需置0
						}
						else
							printf("已拒绝\n");
						cr[i].flag = 1;
					}
					else if(cr[i].type == 32 && cr[i].flag == 0) {
						while(getchar() != '\n');
						printf("%s请求加入群聊:(y/n)(第%d条，还有%d条待处理)\n验证消息%s\n",cr[i].send_user,i+1,cnt-i-1,cr[i].data);
						char c;
						scanf("%c",&c);
						if(c == 'y') {
							memset(&buf,0,sizeof(buf));
							buf.type = 0330;
							//处理申请后发送同意加入请求给服务器
							strcpy(buf.send_user,cr[i].send_user);
							strcpy(buf.recv_user,cr[i].recv_user);
							strcpy(buf.data,cr[i].data);

							buf.flag = 1;

							count = 0;
							//初始化反馈结构体和接收反馈标志
							memset(&back_data,0,sizeof(b_data));
							send(fd,&buf,sizeof(buf),0);
							
							while(1) {
								if(count == 1 && back_data.cnt == 1) {
									printf("对方已加入\n");
									break;
								}
								else if(count == 1 && back_data.cnt == 0) {
									printf("错误\n");
									break;
								}
							}
						}
						else
							printf("已拒绝\n");
						cr[i].flag = 1;
					}
				}
				break;
			case '4':
				while(getchar() != '\n');
				for(i = 0;i < cnt;i++)
					if(cr[i].type == 4) {		//接收文件请求
						char c;
						printf("是否接收来自用户%s的文件(y/n)",cr[i].send_user);
						c = getchar();
						while(getchar() != '\n');
						if(c == 'y') {
							//向服务器发送接收文件的请求
							memset(&buf,0,sizeof(buf));
							buf.type = 0400;
							buf.flag = 1;
							strcpy(buf.send_user,cr[i].send_user);
							strcpy(buf.recv_user,cr[i].recv_user);
							send(fd,&buf,sizeof(buf),0);

							count = 0;
							int size = cr[i].size;
							char *filename = cr[i].data;
							
							//根据文件名创建文件并打开
							int fp = open(filename,O_CREAT | O_APPEND | O_RDWR,0777);
							//开始接收文件
							while(1) {
								if(count == 1 && back_data.ar[0].data[0] != '\0') {
									write(fp,back_data.ar[0].data,50);
									count = 0;	
									printf("#");
									//接收完这一波数据，发送反馈
									//请求接收下一波
									memset(buf.data,0,50);
									send(fd,&buf,sizeof(buf),0);
								}
								else if(count == 1 && strcmp(back_data.ar[0].data,"wanbi") == 0) {
									printf("接收完毕\n");
									break;
								}
							}
						}
						else 
							printf("已拒绝\n");
					}
						default:
							printf("请输入正确的选项\n");
							break;
		}
		while(getchar() != '\n');
		printf("1.私聊\n2.群聊\n3.系统通知\n4.文件传输\nq.退出\n");
	}
	system("clear");
}

//与好友私聊界面
void Chat_Friend(int fd)
{
	request buf;
	char friend_name[20];
	int i;
	char choice2;
	char file[20];

	memset(friend_name,0,20);
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
					printf("%50s:%s\n",back_data.ar[i].data,user);
				else
					printf("%s:%-50s\n",back_data.ar[0].send_user,back_data.ar[i].data);
			}
			break;
		}
		else if(count == 1 && back_data.ar[0].data[0] == '\0')
			break;
	}

	//发送消息
	printf("1.发送消息\n2.发送文件\nq.退出\n");
	strcpy(friend,buf.recv_user);
	while(getchar() != '\n');
	while(scanf("%c",&choice2) && choice2 != 'q') {
		system("clear");
		switch(choice2) {
			case '1':
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
			case '2': {
				memset(&buf,0,sizeof(buf));
				printf("输入文件名:");
				scanf("%s",file);
				//打开当前目录下的此文件，计算其大小，将文件名连同大小发送到服务器
				int filed;
				struct stat auf;
				char c;									//用一字节来传输文件
				int size;						//记录已传输的大小
				if((filed = open(file,O_RDONLY) )== -1) 
					perror("");
				
				if(lstat(file,&auf) == -1) 
					perror("");

				//初始化请求结构体
				strcpy(buf.data,file);
				buf.type = 0400;
				strcpy(buf.recv_user,friend_name);
				buf.flag = 0;
				buf.size = auf.st_size;
				
				count = 0;
				memset(&back_data,0,sizeof(back_data));
				send(fd,&buf,sizeof(buf),0);


				printf("正在等待对方同意\n");
				while(1) {
					printf("#\n");
					sleep(1);
					if(count == 1 && back_data.cnt == 1) {
					//开始传输文件
						memset(&buf,0,sizeof(buf));
						buf.type = 0400;
						buf.flag = 1;
						strcpy(buf.recv_user,friend_name);
						strcpy(buf.send_user,user);
						if(read(filed,buf.data,50) < 1) {	//到末尾结束
							memset(buf.data,0,50);
							strcpy(buf.data,"wanbi");
							send(fd,&buf,sizeof(buf),0);
							printf("发送完毕\n");
							break;
						}
						printf("data = %s\n",buf.data);
						send(fd,&buf,sizeof(buf),0);
						count = 0;
					}
				}
			}
			default:
				printf("请输入正确的选项\n");
				break;
		}
		printf("1.发送消息\n2.发送文件\nq.退出\n");
	}
	system("clear");
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
		else if(count == 1 && back_data.cnt == 0) {
			break;
		}
	}
	if(back_data.cnt == 0) {
		printf("您没有加入该群聊\n");
		return;
	}


	//发送消息
	printf("1.发送消息\n2.踢人\nq.退出\n");
	strcpy(group,buf.recv_user);
	while(getchar() != '\n');				//清空缓冲区	
	while(scanf("%c",&choice2) && choice2 != 'q') {
		system("clear");
		switch(choice2) {
			case '1':
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
		/*	case '2':
				memset(&buf,0,sizeof(buf));		//清空请求结构体
				printf("请输入你想踢走的群成员:");
				scanf("%s",buf.recv_user);
				buf.type = 0372;
				strcpy(buf.send_user,user);
				strcpy(buf.data,group);

				count = 0;
				memset(&back_data,0,sizeof(back_data));
				send(fd,&buf,sizeof(buf),0);
				
				while(1) {
					if(count == 1 && back_data.cnt == 1)
						printf("成功\n");
					else if(count == 1 && back_data.cnt == 0)
						printf("失败\n");
				}
				break;
*/
			default:
				printf("请输入正确的选项\n");
				break;
		}
		while(getchar() !='\n');
		printf("1.发送消息\nq.退出\n");
	}
	system("clear");
	memset(group,0,20);


}
//群聊管理界面
void Group_Manage(int fd)
{
	request buf;
	char choice;
	char create_group[20];
	char add_group[20];
	char delete_group[20];
	char quit_group[20];
	char display_group[20];

	//显示所有已加入的群
	memset(&buf,0,sizeof(buf));
	buf.type = 0350;
	strcpy(buf.send_user,user);
	buf.flag = 0;
	int i;

	count = 0;
	memset(&back_data,0,sizeof(back_data));
	send(fd,&buf,sizeof(buf),0);				//发送请求
	
	while(1) {
		if(count == 1 && back_data.str[0][0] != '\0') {
			for(i = 0;back_data.str[i][0] != '\0';i++)
				printf("%s\n",back_data.str[i]);
			break;
		}
		else if(count == 1 && back_data.str[0][0] == '\0') {
			printf("您无群可水\n");
			break;
		}

	}



	while(getchar() != '\n');
	printf("1.群聊\n2.创建群\n3.解散群\n4.加入群\n5.退出群\n6.查看群成员\nq.退出\n");
	while(scanf("%c",&choice) && choice != 'q') {
		system("clear");
		switch(choice) {
			//群聊
			case '1':
				Chat_Group(fd);
				break;
			//创建群
			case '2':
				printf("请输入要创建的群名称:");
				memset(create_group,0,20);
				scanf("%s",create_group);


				//初始化请求
				memset(&buf,0,sizeof(buf));
				strcpy(buf.data,create_group);
				strcpy(buf.send_user,user);
				buf.type = 0310;

				//发送请求
				count = 0;
				memset(&back_data,0,sizeof(b_data));
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
			//解散群
			case '3':
				printf("请输入要解散的群名称:");
				memset(delete_group,0,20);
				scanf("%s",delete_group);
					
				//初始化请求
				memset(&buf,0,sizeof(buf));
				strcpy(buf.send_user,user);
				strcpy(buf.recv_user,delete_group);
				buf.type = 0320;

				//发送请求
				count = 0;
				memset(&back_data,0,sizeof(b_data));
				send(fd,&buf,sizeof(buf),0);

				while(1) {
					if(count == 1 && back_data.cnt == 1) {
						printf("您的群聊%s解散成功\n",delete_group);
						break;
					}
					else if(count == 1 && back_data.cnt == 0) {
						printf("解散失败\n");
						break;
					}
				}
				break;

			//添加群
			case '4':
				memset(&buf,0,sizeof(buf));
				printf("请输入要水的群名:");
				memset(add_group,0,20);
				scanf("%s",add_group);
				printf("请输入验证消息:");
				scanf("%s",buf.data);

				//初始化请求
				strcpy(buf.recv_user,add_group);
				strcpy(buf.send_user,user);
				buf.fd = fd;
				buf.type = 0330;
				buf.flag = 0;

				//发送请求
				count = 0;
				memset(&back_data,0,sizeof(b_data));
				send(fd,&buf,sizeof(buf),0);

				while(1) {
					if(count == 1 && back_data.cnt == 1) {
						printf("申请已发送成功，等待同意\n");
						break;
					}
					else if(count == 1 && back_data.cnt == 0) {
						printf("申请发送失败!\n");
						break;
					}
				}
				break;
			//退出群聊
			case '5':
				memset(quit_group,0,20);
				memset(&buf,0,sizeof(buf));
				memset(&back_data,0,sizeof(b_data));

				printf("请输入要退出的群聊名称:");
				scanf("%s",quit_group);
				strcpy(buf.recv_user,quit_group);

				buf.type = 0340;
				strcpy(buf.send_user,user);
				buf.flag = 0;

				count = 0;
				memset(&back_data,0,sizeof(b_data));
				send(fd,&buf,sizeof(buf),0);
				while(1) {
					if(count == 1 && back_data.cnt == 1) {
						printf("退出成功\n");
						break;
					}
					else if(count == 1 && back_data.cnt == 0) {
						printf("退出失败\n");
						break;
					}
				}
				break;
			//查看群成员
			case '6':
				memset(display_group,0,20);
				memset(&buf,0,sizeof(buf));
				memset(&back_data,0,sizeof(b_data));

				printf("请输入要查看的群聊名称:");
				scanf("%s",display_group);
				strcpy(buf.recv_user,display_group);

				buf.type = 0360;
				strcpy(buf.send_user,user);
				buf.flag = 0;
				
				count = 9;
				memset(&back_data,0,sizeof(b_data));
				send(fd,&buf,sizeof(buf),0);
				while(1) {
					if(count == 1 && back_data.str[0][0] != '\0') {
						for(int i = 0;back_data.str[i][0] != '\0';i++)
							printf("%s\n",back_data.str[i]);
						break;
					}
					else if(count == 1 && back_data.str[0][0] == '\0') {
						printf("无成员\n");
						break;
					}
				}
				break;
			default:
				printf("请输入正确的选项\n");
				break;
		}
		while(getchar() != '\n');			//清空缓冲区
		printf("1.群聊\n2.创建群\n3.解散群\n4.加入群\n5.退出群\n6.查看群成员\nq.退出\n");
	}
	system("clear");
}



