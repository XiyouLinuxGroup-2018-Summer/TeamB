#ifndef QQ
#define QQ
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
#include <mysql.h>

//请求类型结构体
typedef struct reque{
	int type;								//请求类型
	char send_user[20];						//请求方
	int fd;									//请求方套接字
	char recv_user[20];						//被请求方
	char  data[50];							//请求数据
	time_t time;							//请求时间
	int flag;								//是否被处理
	int size;								//请求数据大小
}request;

//消息记录结构体
struct record{
	char send_user[20];						//发送方
	char recv_user[20];						//接收方
	time_t time;							//发送时间
	int type;								//消息性质1,私聊2.群聊3.系统通知4.文件
	char data[50];							//消息内容
	int flag;								//是否被处理
	int size;								//消息大小
};


//消息类型：31添加好友；32入群申请 ;33解散群;4接收文件
//反馈消息类型： 431 添加好友; 432加入群申请;433解散群通知;434踢人通知	
//反馈数据类型结构体
typedef struct dat{
	int type;								//反馈消息类型
	int cnt;								//是否成功			1
	char passwd[20];						//找回密码			2
	char str[100][20];						//显示所有好友		3
	struct record ar[50];					//消息记录			4
	int flag;								//是否被处理
	int size;								//反馈数据大小
}b_data;

//根据用户名得到对应的套接字
int get_fd(char *name);

//处理踢人请求					
void Kick_member(request buf,b_data *back_data);

//显示某个用户是否在线
int Online(char *name);

//处理离线消息
void New_offline(b_data back_data,int fd);

//处理发送文件请求
void Send_file(request buf,b_data *back_data);

//处理发送群聊消息请求
void Send_gmessage(request buf,b_data *back_data);

//处理发送私人消息请求
void Send_message(request buf,b_data *back_data);

//处理添加好友请求
void Add_Friend(request buf,b_data * back_data);

//处理删除好友请求
void Delete_Friend(request buf,b_data *back_data);

//处理退出群聊请求
void Quit_Group(request buf,b_data *back_data);

//处理解散群请求
void Delete_Group(request buf,b_data *back_data);

//处理查看私人聊天记录请求
void View_record(request buf,b_data *back_data);

//处理创建群聊请求
void Create_Group(request buf,b_data *back_data);

//处理查看群成员请求
void Show_member(request buf,b_data *back_data);

//处理查看群聊天记录请求
void View_grecord(request buf,b_data *back_data);

//处理添加群请求
void Add_Group(request buf,b_data *back_data);

//处理显示所有群请求
void display_all(request buf,b_data *back_data);

//处理显示所有好友请求
void display_all(request buf,b_data *back_data);

//处理登录请求
int login_server(request buf);

//处理注册请求
int register_server(request buf);

//处理所有请求
int login_server(request buf);

//管理界面，包括好友管理，群聊管理，消息管理，账号管理
void Main_menu(int fd);

//好友管理界面
void Friend_Manage(int fd);

//消息管理界面
void News_Manage(int fd);

//好友聊天界面
void Chat_Friend(int fd);

//群聊管理界面
void Group_Manage(int fd);

//处理注销请求
void Quit(request buf);
#endif
