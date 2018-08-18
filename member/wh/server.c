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
#include "myQQ.h"
#define SERV_PORT				5678		//服务器端端口


MYSQL mysql;

//处理发送群聊消息请求
void Send_gmessage(request buf,b_data *back_data)
{
	MYSQL_RES	*res = NULL;
	MYSQL_ROW	rows;
	int rc,i,fields;
	char query_str[200];
	int row;
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"select * from relationinfo where name2='%s'",buf.recv_user);	//找到此群聊中所有成员，规定关系信息表中群聊在字段2
	
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	res = mysql_store_result(&mysql);
	fields = mysql_num_fields(res);
	row = mysql_num_rows(res);
	char ptr[row][20];
	for(i = 0;i < row;i++) {
		rows = mysql_fetch_row(res);
		memset(ptr[i],0,20);
		strcpy(ptr[i],rows[0]);
	}
		
	

	//对每个除了自己以外的成员发送消息
	for(i = 0;i < row;i++) {
		if(strcmp(ptr[i],buf.send_user)) {
			memset(query_str,0,strlen(query_str));
			sprintf(query_str,"select * from userinfo where name='%s'",ptr[i]);
			rc = mysql_real_query(&mysql,query_str,strlen(query_str));

			//判断对方是否离线
			res = mysql_store_result(&mysql);
			fields = mysql_num_fields(res);
			rows = mysql_fetch_row(res);

			if(rows[3][0] - '0' == 0) {
				back_data->cnt = 1;
				break;
			}
			
			int	recv_fd = rows[2][0] - '0';
		
			//将消息写入群聊天记录表中
			memset(query_str,0,strlen(query_str));
			sprintf(query_str,"insert into recordinfo(send_user,recv_user,type,data) values('%s','%s',%d,'%s')",buf.send_user,buf.recv_user,42,buf.data);
			rc = mysql_real_query(&mysql,query_str,strlen(query_str));

			//接收方存在且在线
			back_data->cnt = 1;
			b_data auf;
			memset(&auf,0,sizeof(auf));
			auf.type = 42;

			strcpy(auf.ar[0].send_user,buf.send_user);
			strcpy(auf.ar[0].recv_user,buf.recv_user);
			auf.ar[0].type = 2;
			strcpy(auf.ar[0].data,buf.data);
			printf("%s------->>>>%s\n",buf.send_user,buf.data);
			send(recv_fd,&auf,sizeof(b_data),0);	//发送消息
		}
	}

}


//处理发送私聊消息请求
void Send_message(request buf,b_data *back_data)
{
	MYSQL_RES	*res = NULL;
	MYSQL_ROW	rows;
	int rc,i,fields;
	char query_str[200];
	int recv_fd;
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"select * from userinfo where name='%s'",buf.recv_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	
	//判断对方是否离线
	res = mysql_store_result(&mysql);	
	fields = mysql_num_fields(res);
	rows = mysql_fetch_row(res);

	if(rows[3][0] - '0'== 0) {
		back_data->cnt = 1;				//搁置请求，等对方上线再发送
		return;
	}

	recv_fd = atoi(rows[2]);
	printf("recv_fd = %d\n",recv_fd);

	//将消息写入聊天记录表中
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"insert into recordinfo(send_user,recv_user,type,data) values('%s','%s',%d,'%s')",buf.send_user,buf.recv_user,41,buf.data);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));

	//接收方存在且在线
	back_data->cnt = 1;
	b_data auf;
	memset(&auf,0,sizeof(b_data));
	auf.type = 41;
	
	strcpy(auf.ar[0].send_user,buf.send_user);
	strcpy(auf.ar[0].recv_user,buf.recv_user);
	auf.ar[0].type = 1;
	strcpy(auf.ar[0].data,buf.data);
	send(recv_fd,&auf,sizeof(b_data),0);//发送消息


}

//处理删除好友请求
void Delete_Friend(request buf,b_data *back_data)
{
	MYSQL_RES	*res = NULL;
	MYSQL_ROW	rows;
	int rc,i,fields;
	int row;
	char query_str[200];					//存放mysql语句
	int recv_fd;
	//判断双方是否为好友
	sprintf(query_str,"select * from relationinfo where name1='%s' and name2='%s'",buf.send_user,buf.recv_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc != 0) {
		back_data->cnt = 0;					//说明双方不是好友了
		puts("1");
		return;	
	}
	printf("%s %s\n",buf.send_user,buf.recv_user);
	
	res = mysql_store_result(&mysql);
	mysql_free_result(res);
	
	
	//从关系信息表中删除双方好友信息
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"delete from relationinfo where name1='%s' and name2='%s'",buf.send_user,buf.recv_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc != 0) 
		printf("%s\n",mysql_error(&mysql));
	
	res = mysql_store_result(&mysql);

	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"delete from relationinfo where name1='%s' and name2='%s'",buf.recv_user,buf.send_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc != 0) 
		printf("%s\n",mysql_error(&mysql));
	back_data->cnt = 1;

}

//处理创建群聊请求
void Create_Group(request buf,b_data *back_data)
{
	MYSQL_RES	*res = NULL;
	
	int rc,i,fields;
	int rows;
	char query_str[200];
	int recv_fd;
	//判断是否存在此群聊
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"select * from relationinfo where name2='%s'",buf.data);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	res = mysql_store_result(&mysql);
	rows = mysql_num_rows(res);

	//判断该群聊是否存在
	if(rows != 0) {
		back_data->cnt = 0;
		return;
	}

	//若不存在则创建
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"insert into relationinfo(name1,name2,power) values('%s','%s',%d)",buf.send_user,buf.data,1);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc != 0) {
		back_data->cnt = 0;
		return;
	}
	back_data->cnt = 1;

}

//处理添加好友请求
void Add_Friend(request buf,b_data * back_data)
{
	
	MYSQL_RES	*res = NULL;
	MYSQL_ROW	rows;
	int rc,i,fields;
	int row;
	char query_str[200];					//存放mysql语句
	int recv_fd;
	//判断双方是否为好友
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"select * from relationinfo where name1='%s' and name2='%s'",buf.send_user,buf.recv_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc == 0) {
		back_data->cnt = 0;					//说明双方已经是好友了
		return;	
	}

	//判断对方是否离线
	mysql_store_result(&mysql);
	memset(query_str,0,sizeof(strlen(query_str)));
	sprintf(query_str,"select * from userinfo where name='%s'",buf.recv_user);
	//从用户信息表中获取接收方信息
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc != 0) {								//说明该用户不存在
		back_data->cnt = 0;
		return;
	}

	res = mysql_store_result(&mysql);
	row = mysql_num_rows(res);
	fields = mysql_num_fields(res);
	rows = mysql_fetch_row(res);
	if(strcmp(rows[3],"0") == 0)	{									//离线状态
		back_data->cnt = 1;				//搁置请求，等对方上线再发送
		return;
	}
	recv_fd = rows[2][0] - '0';
	//接收方存在且在线
	back_data->cnt = 1;
	b_data auf;
	memset(&auf,0,sizeof(b_data));
	auf.type = 431;
	strcpy(auf.ar[0].send_user,buf.send_user);
	strcpy(auf.ar[0].recv_user,buf.recv_user);
	auf.ar[0].type = 3;
	strcpy(auf.ar[0].data,buf.data);
	send(recv_fd,&auf,sizeof(b_data),0);//发送请求

}
//处理查看群聊天记录请求
//参数：用户名，群名称
//反馈数据：返回该群聊的聊天记录，存入back_data中
void View_grecord(request buf,b_data *back_data)
{
	MYSQL_RES	*res = NULL;
	MYSQL_ROW	rows;
	int row;
	char query_str[200];					//存放mysql语句的字符串数组
	int rc,i,fields;
	int count = 0;
	//在表中查找所有与该群聊有关的聊天记录

	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"select * from recordinfo where recv_user='%s'",buf.recv_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc != 0) {
		printf("error:%s",mysql_error(&mysql));
	}

	res = mysql_store_result(&mysql);
	row = mysql_num_rows(res);
	fields = mysql_num_fields(res);
	while((rows = mysql_fetch_row(res))) {
		strcpy(back_data->ar[count].send_user,rows[0]);
		strcpy(back_data->ar[count].recv_user,rows[1]);
		back_data->ar[count].type = 2;
		strcpy(back_data->ar[count].data,rows[4]);
		count++;
	}

}	
//处理查看私人聊天记录请求
//参数;用户名，好友名
//反馈数据：返回该用户与该好友的聊天记录信息，存入back_data中
void View_record(request buf,b_data *back_data)
{
	MYSQL_RES	*res = NULL;
	MYSQL_ROW	rows;
	int row;
	char query_str[200];					//存放mysql语句的字符串数组
	int rc,i,fields;
	int count = 0;
	//在表中查找所有与该用户和该好友有关的聊天记录

	char *name1 = buf.send_user;
	char *name2 = buf.recv_user;
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"select * from recordinfo where send_user='%s' or recv_user='%s'",name1,name1);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc != 0) {
		printf("error:%s",mysql_error(&mysql));
	}

	res = mysql_store_result(&mysql);
	row = mysql_num_rows(res);
	fields = mysql_num_fields(res);
	while((rows = mysql_fetch_row(res))) {
			if(strcmp(rows[0],name1) == 0 && strcmp(rows[1],name2) == 0) {
				strcpy(back_data->ar[count].send_user,rows[0]);
				strcpy(back_data->ar[count].recv_user,rows[1]);
				back_data->ar[count].type = 1;
				strcpy(back_data->ar[count].data,rows[4]);
				count++;
			}
				
			else if(strcmp(rows[0],name2) == 0 && strcmp(rows[1],name1) == 0) {
				strcpy(back_data->ar[count].send_user,rows[0]);
				strcpy(back_data->ar[count].recv_user,rows[1]);
				back_data->ar[count].type = 1;
				strcpy(back_data->ar[count].data,rows[4]);
				count++;
			}
	}
}

//显示某个用户是否在线
//参数：用户名
//若在线，返回1，若不在线，返回2
/*int Online(char *name)
{
	char query_str[200];
	memset(query_str,0,strlen(query_str));

	sprintf
*/



//处理显示所有好友请求
//参数：用户名
//反馈数据：返回所有该用户的好友信息,存入back_data中
void display_all(request buf,b_data *back_data)
{
	MYSQL_RES	*res = NULL;
	MYSQL_ROW	rows;
	int row;
	char query_str[200];					//存放mysql语句的字符串数组
	int rc,i,fields;
	char ptr[100][20];						//反馈类型
	int count = 0;
	//在表中查找所有与该用户名是好友的用户
	

	char *name = buf.send_user;
	int fd = buf.fd;
	
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"select *%c from relationinfo",' ');
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc != 0) {
		printf("mysql_read_query():%s\n",mysql_error(&mysql));
	}
	
	res = mysql_store_result(&mysql);
	if(res == NULL) {
		printf("error:%s",mysql_error(&mysql));
	}
	row = mysql_num_rows(res);
	fields = mysql_num_fields(res);
	while((rows = mysql_fetch_row(res))) {
			if(strcmp(rows[0],buf.send_user) == 0)
				strcpy(back_data->str[count++],rows[1]);
	}
	
}
			

//处理登录请求
//参数：用户名name 密码 passwd
//反馈数据：在用户文件里面找到则返回int 1，否则返回0
int login_server(request buf)
{
	MYSQL_RES *res = NULL;
	int rows;
	char query_str[200];						//存放mysql语句的字符串数组
	int rc;
	int feedback = 0;							//反馈类型
	//在文件中查找是否存在该用户名以及对应的密码
	
	char *name = buf.send_user;
	char *passwd = buf.data;
	int fd = buf.fd;				
	memset(query_str,0,strlen(query_str));
	printf("name = %s,passwd =%s\n",name,passwd);
	sprintf(query_str,"select * from userinfo where name='%s' and passwd='%s'",name,passwd);
	
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	memset(query_str,0,strlen(query_str));		//清空语句
	res = mysql_store_result(&mysql);
	rows = mysql_num_rows(res);
	
	if(rows != 0) {
		feedback = 1;
		//登录成功，将用户信息表中用户在线状态改为在线,套接字改为此次登录的所使用的套接字
		sprintf(query_str,"update userinfo set state=%d,fd=%d where name='%s'",1,fd,name);
		mysql_store_result(&mysql);
		rc = mysql_real_query(&mysql,query_str,strlen(query_str));
		if(rc != 0) {
			printf("%s",mysql_error(&mysql));
			printf("登录写入失败\n");
		}
		memset(query_str,0,strlen(query_str));
	}
	return feedback;	
}

//处理注册请求
//参数：新用户名name新密码 passwd
//反馈数据：注册成功返回1，否则返回0
int register_server(request buf)
{
	char query_str[200];
	int feedback = 0;							//反馈类型
	//在文件中查找是否存在该用户名
	int rc;
	
	char *name = buf.send_user;
	char *passwd = buf.data;
	int fd = buf.fd;

	sprintf(query_str,"select * from userinfo where='%s'",name);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	memset(query_str,0,strlen(query_str));
	if(rc != 0) {
		feedback = 1;
		//注册成功，将新用户信息写入用户信息表
		sprintf(query_str,"insert into userinfo values('%s','%s',%d,%d)",name,passwd,fd,0);
		rc = mysql_real_query(&mysql,query_str,strlen(query_str));
		if(rc != 0) 
			printf("注册写入失败\n");
		memset(query_str,0,strlen(query_str));
	}
	return feedback;	
}

//处理所有请求的函数
//参数，客户端套接字
//反馈数据：无
void *handle_all(void *fd)					//int fd
{
	
	pthread_t thid;
	request buf;
	b_data back_data;
	int conn_fd = *(int *)fd;
	while(1) {
		int ret = recv(conn_fd,&buf,sizeof(buf),0);
		if(ret < 0) {
			perror("recv:");
		}
		else if(ret == 0) 
			pthread_exit(0);
		switch(buf.type) {
			case 0100:
				//处理登录请求
				memset(&back_data,0,sizeof(b_data));
				buf.fd = conn_fd;
				back_data.cnt = login_server(buf);
				send(conn_fd,&back_data,sizeof(b_data),0);
				break;
			case 0110:
				//处理注册请求
				memset(&back_data,0,sizeof(b_data));
				back_data.cnt = register_server(buf);
				send(conn_fd,&back_data,sizeof(b_data),0);
				break;
			case 0240:
				//处理显示所有好友请求
				printf("%s\n",buf.send_user);
				memset(&back_data,0,sizeof(b_data));
				display_all(buf,&back_data);
				send(conn_fd,&back_data,sizeof(b_data),0);
				break;
			case 0250:
				//处理查看私人聊天记录请求
				printf("%s %s\n",buf.send_user,buf.recv_user);
				memset(&back_data,0,sizeof(b_data));
				View_record(buf,&back_data);
				send(conn_fd,&back_data,sizeof(b_data),0);
				break;
			case 0370:
				//处理查看群聊天记录请求
				printf("%s %s\n",buf.send_user,buf.recv_user);
				memset(&back_data,0,sizeof(b_data));
				View_grecord(buf,&back_data);
				send(conn_fd,&back_data,sizeof(b_data),0);
				break;
			case 0310:
				//处理创建群聊请求
				printf("%s %s\n",buf.send_user,buf.data);
				memset(&back_data,0,sizeof(b_data));
				Create_Group(buf,&back_data);
				send(conn_fd,&back_data,sizeof(b_data),0);
				break;
			case 0210:
				//处理添加好友请求
				printf("%s %s\n",buf.send_user,buf.recv_user);
				memset(&back_data,0,sizeof(b_data));
				Add_Friend(buf,&back_data);
				send(conn_fd,&back_data,sizeof(b_data),0);
				break;
			case 0220:
				//处理删除好友请求
				printf("%s %s\n",buf.send_user,buf.recv_user);
				memset(&back_data,0,sizeof(b_data));
				Delete_Friend(buf,&back_data);
				send(conn_fd,&back_data,sizeof(b_data),0);
				break;
			case 0200:
				//处理发送消息请求
				printf("%s %s\n",buf.send_user,buf.recv_user);
				memset(&back_data,0,sizeof(b_data));
				Send_message(buf,&back_data);
				send(conn_fd,&back_data,sizeof(b_data),0);
			case 0300:
				//处理发送群聊消息请求
				printf("send_user = %s recv_user = %s\n",buf.send_user,buf.recv_user);
				memset(&back_data,0,sizeof(b_data));
				Send_gmessage(buf,&back_data);
				send(conn_fd,&back_data,sizeof(b_data),0);
				break;

			default:
				//
				break;
		}
	}

}

int main(int argc,char *argv[])
{
	if(mysql_init(&mysql) == NULL) {					//分配和初始化mysql对象
		printf("mysql_init():%s\n",mysql_error(&mysql));
		return -1;
	}
														//连接
	mysql_real_connect(&mysql,NULL,"root","123456","wh",0,NULL,0);
	struct sockaddr_in serv_addr,cli_addr;
	int sock_fd = socket(AF_INET,SOCK_STREAM,0);		//创建TCP套接字
	if(sock_fd < 0) {
		perror("socket :");
	}

	//初始化服务器端地址结构
	memset(&serv_addr,0,sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;						
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//将套接字绑定到本地端口
	if(bind(sock_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in)) < 0) {
			perror("bind:");
	}

	//转化为监听套接字
	if(listen(sock_fd,100) < 0) {
		perror("listen:");
	}
	int cli_len = sizeof(cli_addr);
	int *conn_fd;
	while(1) {
		conn_fd = (int *)malloc(sizeof(int));
		//接受来自客户端的连接请求
		*conn_fd = accept(sock_fd,(struct sockaddr *)&cli_addr,&cli_len);
		printf("welcome:%d\n",*conn_fd);
		if(conn_fd < 0) {
			perror("accept:");
		}

		//创建一个线程去处理连接
		pthread_t thid;
		pthread_create(&thid,NULL,handle_all,(void *)conn_fd);
	}
	return 0;
}

