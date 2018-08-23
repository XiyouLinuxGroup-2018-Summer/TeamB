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

/*//处理踢人请求
void Kick_member(request buf,b_data *back_data)
{
	//查看发送方是否具有权限
	MYSQL_RES *res = NULL;
	MYSQL_ROW rows;
	char query_str[200];
	memset(query_str,0,200);
	sprintf(query_str,"select * from relationinfo where name1='%s' and name2='%s' and (power=1 or power=2)",buf.send_user,buf.data);
	mysql_real_query(&mysql,query_str,strlen(query_str));
	res = mysql_store_result(&mysql);
	rows = mysql_fetch_row(res);
	int num = mysql_num_rows(res);
	if(num == 0) {
		back_data->cnt = 0;
		return;
	}

	//从用户关系信息表中删除
	memset(query_str,0,200);
	sprintf(query_str,"delete from relationinfo where name1='%s' and name2='%s'",buf.send_user,buf.data);
	mysql_real_query(&mysql,query_str,strlen(query_str));
	
	//发送信息给被踢的用户
	b_data auf;
	memset(&auf,0,sizeof(auf));
	auf.type = 434;
	strcpy(auf.ar[0].send_user,buf.send_user);
	strcpy(auf.ar[0].recv_user,buf.recv_user);
	strcpy(auf.ar[0].data,buf.data);
	
	if(Online(buf.recv_user) == 0) {	//离线状态	
		back_data->cnt = 1;
	(auf,0);
		return;
	}

	send(get_fd(buf.recv_user),&auf,sizeof(auf),0);

}*/
//根据用户名得到对应的套接字
int get_fd(char *name)
{
	//根据用户名得到接收方套接字
	MYSQL_RES *res = NULL;
	MYSQL_ROW rows;
	char query_str[200];
	memset(query_str,0,200);
	sprintf(query_str,"select * from userinfo where name='%s'",name);
	mysql_real_query(&mysql,query_str,strlen(query_str));
	res = mysql_store_result(&mysql);
	rows = mysql_fetch_row(res);


	return rows[2][0] - '0';
}
void Quit(request buf)
{
	MYSQL_RES *res = NULL;
	MYSQL_ROW	rows;
	char query_str[200];

	//改变用户信息表中此用户状态
	memset(query_str,0,200);
	sprintf(query_str,"update userinfo set state=0   where name='%s'",buf.send_user);
	int rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc != 0)
		printf(" %s\n",mysql_error(&mysql));
}

//处理发送消息时对方离线的问题
void New_offline(b_data back_data,int fd)
{
	MYSQL_RES	*res = NULL;
	MYSQL_ROW	rows;
	int rc,row;
	char query_str[200];
	if(back_data.ar[0].flag == 0) {	
	//将离线消息放入离线消息记录表中
		memset(query_str,0,200);
		sprintf(query_str,"insert into off_recordinfo(send_user,recv_user,type,data) values('%s','%s',%d,'%s')",back_data.ar[0].send_user,back_data.ar[0].recv_user,back_data.type,back_data.ar[0].data);
		rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	}
	
	if(back_data.ar[0].flag == 1) {
		//打开离线消息表，对比是否有需要发送的离线消息
		memset(query_str,0,200);
		sprintf(query_str,"select * from off_recordinfo where recv_user='%s'",back_data.ar[0].send_user);
		rc = mysql_real_query(&mysql,query_str,strlen(query_str));
		res = mysql_store_result(&mysql);
		int n = mysql_num_rows(res);
		if(n == 0)
			return;

		printf(" n = %d\n",row);
		if(n != 0) {								//存在需要发送的离线消息
			b_data auf;
			while(rows = mysql_fetch_row(res)) {
				memset(&auf,0,sizeof(auf));
				//初始化消息结构体
				
				auf.type = atoi(rows[2]);
				if(auf.type == 432) {
					strcpy(auf.ar[0].send_user,rows[0]);
					strcpy(auf.ar[0].recv_user,rows[3]);
				}
				else {
					strcpy(auf.ar[0].send_user,rows[0]);
					strcpy(auf.ar[0].recv_user,rows[1]);
				}

				send(fd,&auf,sizeof(auf),0);
			}
			memset(query_str,0,200);
			sprintf(query_str,"delete from off_recordinfo where recv_user='%s'",back_data.ar[0].send_user);
			mysql_real_query(&mysql,query_str,strlen(query_str));
		}
	}




}


//处理发送文件请求
void Send_file(request buf,b_data *back_data)
{
	
	
	if(buf.flag == 0) {					//第一次开始准备发送文件
		back_data->type = 0400;		//返回值类型为文件	

		//将信息存入新的反馈结构体并发送给另一个客户端
		b_data auf;
		memset(&auf,0,sizeof(auf));
		auf.type = 44;
		strcpy(auf.ar[0].send_user,buf.send_user);
		strcpy(auf.ar[0].recv_user,buf.recv_user);
		strcpy(auf.ar[0].data,buf.data);
		auf.ar[0].type = 4;
		auf.flag = 0;
		auf.ar[0].size = buf.size;

		if(Online(buf.recv_user) == 0) {	//对方未在线
			New_offline(auf,0);		//信息存入离线消息表中
			return;	
		}
		//发送接收文件大小以及名称给接收客户端
		printf("s:%s r:%s da:%s\n",buf.send_user,buf.recv_user,buf.data);
		send(get_fd(buf.recv_user),&auf,sizeof(auf),0);
		
		return;
	}
	if(buf.flag == 1 && buf.data[0] == '\0') {	
		//发送接收反馈给发送方
		if(buf.size == 0) {
			back_data->cnt = 1;
			send(get_fd(buf.send_user),back_data,sizeof(b_data),0);
		}
		else {
			back_data->cnt = 0;
			send(get_fd(buf.send_user),back_data,sizeof(b_data),0);
		}
	}
	else if(buf.flag == 1 && buf.size != 0) {
		//将文件内容传输到接收方	
		b_data auf;
		memset(&auf,0,sizeof(auf));
		if(buf.fd == -2)
			buf.size = 0;
		memcpy(auf.ar[0].data,buf.data,buf.size);
		auf.size = buf.size;
		printf("auf.size = %ld",auf.size);
		
		send(get_fd(buf.recv_user),&auf,sizeof(b_data),0);
		back_data->cnt = 1;
	}	
}
//处理发送群聊消息请求
void Send_gmessage(request buf,b_data *back_data)
{
	MYSQL_RES	*res = NULL;
	MYSQL_ROW	rows;
	int rc,i,fields;
	char query_str[200];
	int row;
	//判断该用户是否在群聊中
	memset(query_str,0,200);
	sprintf(query_str,"select * from relationinfo where name1='%s' and name2='%s'",buf.send_user,buf.recv_user);
	mysql_real_query(&mysql,query_str,strlen(query_str));
	res = mysql_store_result(&mysql);
	row = mysql_num_rows(res);
	
	if(row == 0) {				//该用户不在群聊中
		back_data->cnt = 0;
		return;
	}
	
	//将消息写入群聊天记录表中	
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"insert into recordinfo(send_user,recv_user,type,data) values('%s','%s',%d,'%s')",buf.send_user,buf.recv_user,42,buf.data);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));


	
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
			
			res = mysql_store_result(&mysql);
			fields = mysql_num_fields(res);
			rows = mysql_fetch_row(res);


			int	recv_fd = rows[2][0] - '0';
		

			//处理要发送的消息
			back_data->cnt = 1;
			b_data auf;
			memset(&auf,0,sizeof(auf));
			auf.type = 42;

			strcpy(auf.ar[0].send_user,buf.send_user);
			strcpy(auf.ar[0].recv_user,buf.recv_user);
			auf.ar[0].type = 2;
			auf.flag = 0;
			strcpy(auf.ar[0].data,buf.data);
			//判断对方是否离线
				if(rows[3][0] - '0' == 0) {
				back_data->cnt = 1;
				New_offline(auf,0);
				continue;
			}
			
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
	//判断双方是否为好友
	
	sprintf(query_str,"select * from relationinfo where (name1='%s' and name2='%s') or (name1='%s' and name2='%s')",buf.send_user,buf.recv_user,buf.recv_user,buf.send_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	res = mysql_store_result(&mysql);
	int n = mysql_num_rows(res);

	if(n == 0) {
		back_data->cnt = 0;					//说明双方不是好友了
		return;	
	}
	
	//将消息写入聊天记录表中
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"insert into recordinfo(send_user,recv_user,type,data) values('%s','%s',%d,'%s')",buf.send_user,buf.recv_user,41,buf.data);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	

	back_data->cnt = 1;
	b_data auf;
	memset(&auf,0,sizeof(b_data));
	auf.type = 41;
	
	strcpy(auf.ar[0].send_user,buf.send_user);
	strcpy(auf.ar[0].recv_user,buf.recv_user);
	auf.ar[0].type = 1;
	auf.flag = 0;
	strcpy(auf.ar[0].data,buf.data);
	
	//判断对方是否离线
	if(Online(buf.recv_user) == 0) {
		back_data->cnt = 1;
		New_offline(auf,0);
		return;
	}
	send(get_fd(buf.recv_user),&auf,sizeof(b_data),0);//发送消息


}

//处理解散群请求
void Delete_Group(request buf,b_data *back_data)
{
	MYSQL_RES	*res = NULL;
	MYSQL_ROW	rows;
	int rc,i,fields;
	int row;
	char query_str[200];					//存放mysql语句
	int recv_fd;
	//判断申请用户是否为群主
	sprintf(query_str,"select * from relationinfo where name1='%s' and name2='%s' and power=1",buf.send_user,buf.recv_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	res = mysql_store_result(&mysql);
	int num = mysql_num_rows(res);
	if(num == 0) {
		back_data->cnt = 0;					//说明不是群主
		return;	
	}
	
	res = mysql_store_result(&mysql);
	mysql_free_result(res);
	
	
	//向所有群成员发送解散群通知
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"select * from relationinfo where name2='%s'",buf.recv_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc != 0) 
		printf("%s\n",mysql_error(&mysql));
	
	res = mysql_store_result(&mysql);
	while(rows = mysql_fetch_row(res)) {

		b_data auf;
		memset(&auf,0,sizeof(b_data));
		auf.type = 433;
		auf.flag = 0;
		strcpy(auf.ar[0].send_user,buf.send_user);
		strcpy(auf.ar[0].recv_user,buf.recv_user);
		auf.ar[0].type = 33;
		
		//判断对方是否离线
		if(Online(rows[0]) == 0) {
			back_data->cnt = 1;
			New_offline(auf,0);
			continue;
		}
	
		//发送成功，反馈值置于1
		back_data->cnt = 1;
		
		send(get_fd(rows[0]),&auf,sizeof(b_data),0);
	}
	//删除用户关系表中所有该群信息
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"delete from relationinfo where name2='%s'",buf.recv_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc != 0)
		printf("%s\n",mysql_error(&mysql));
	
}		

//处理退出群聊请求
void Quit_Group(request buf,b_data *back_data)
{
	MYSQL_RES	*res = NULL;
	MYSQL_ROW	rows;
	int rc,i,fields;
	int row;
	char query_str[200];					//存放mysql语句
	int recv_fd;
	//判断是否在该群聊中
	sprintf(query_str,"select * from relationinfo where name1='%s' and name2='%s'",buf.send_user,buf.recv_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	res = mysql_store_result(&mysql);
	rows = mysql_fetch_row(res);

	int n = mysql_num_rows(res);
	
	if(n == 0) {
		back_data->cnt = 0;					//说明没在该群聊中
		return;	
	}
	if(strcmp(rows[2],"1") == 0) {			//说明为群主
		back_data->cnt = 0;
		return;
	}
	
	
	res = mysql_store_result(&mysql);


	//发送退出消息给所有管理员和群主
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"select * from relationinfo where name2='%s'",buf.recv_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));

	res = mysql_store_result(&mysql);
	row = mysql_num_rows(res);
	int count = mysql_num_rows(res);
	printf("count = %d\n",count);

	while(rows = mysql_fetch_row(res)) {
		printf("%s -====================\n",rows[0]);
		if(!strcmp(rows[2],"1") || !strcmp(rows[2],"2")) {		//找到群主和管理员

			b_data auf;
			memset(&auf,0,sizeof(b_data));
			auf.type = 434;
			strcpy(auf.ar[0].send_user,buf.send_user);
			strcpy(auf.ar[0].recv_user,buf.recv_user);
			strcpy(auf.ar[0].data,buf.data);
			auf.ar[0].type = 34;
			auf.flag == 0;

			//判断对方是否离线
			if(Online(rows[2]) == 0) {
				back_data->cnt = 1;
				New_offline(auf,0);
				continue;
			}
			send(get_fd(rows[2]),&auf,sizeof(b_data),0);	

		}
		//发送成功，反馈值置1
		back_data->cnt = 1;
	}
	
	//从关系信息表中删除该用户与群聊的信息
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"delete from relationinfo where name1='%s' and name2='%s'",buf.send_user,buf.recv_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc != 0) 
		printf("%s\n",mysql_error(&mysql));

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
	sprintf(query_str,"select * from relationinfo where (name1='%s' and name2='%s') or (name1='%s' and name2='%s')",buf.send_user,buf.recv_user,buf.recv_user,buf.send_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	res = mysql_store_result(&mysql);
	int n = mysql_num_rows(res);
	if(n == 0) {
		back_data->cnt = 0;					//说明双方不是好友了
		return;	
	}
	
	res = mysql_store_result(&mysql);
	mysql_free_result(res);
	
	
	//从关系信息表中删除双方好友信息
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"delete from relationinfo where (name1='%s' and name2='%s') or (name1='%s' and name2='%s')",buf.send_user,buf.recv_user,buf.recv_user,buf.send_user);
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
	sprintf(query_str,"insert into relationinfo(name1,name2,power) values('%s','%s',1)",buf.send_user,buf.data);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc != 0) {
		back_data->cnt = 0;
		return;
	}
	back_data->cnt = 1;

}

//处理添加群请求
void Add_Group(request buf,b_data *back_data)
{
	MYSQL_RES	*res = NULL;
	MYSQL_ROW	rows;
	int rc,i,fields;
	int row;
	char query_str[200];					//存放mysql语句
	int recv_fd;
	
	if(buf.flag == 0) {							//第一次提交申请
		//判断是否在群聊里面
		memset(query_str,0,strlen(query_str));
		sprintf(query_str,"select * from relationinfo where name1='%s' and name2='%s'",buf.send_user,buf.recv_user);
		rc = mysql_real_query(&mysql,query_str,strlen(query_str));
		res = mysql_store_result(&mysql);
		row = mysql_num_rows(res);

		if(row != 0) {
			back_data->cnt = 0;					//说明已经在群里了
			return;	
		}
		//找出所有管理员和群主
		memset(query_str,0,strlen(query_str));
		sprintf(query_str,"select * from relationinfo where name2='%s'",buf.recv_user);
		rc = mysql_real_query(&mysql,query_str,strlen(query_str));

		res = mysql_store_result(&mysql);
		row = mysql_num_rows(res);
		int count = mysql_num_rows(res);
		while(rows = mysql_fetch_row(res)) {

			if(!strcmp(rows[2],"1") || !strcmp(rows[2],"2")) {		//找到群主和管理员
				b_data auf;
				memset(&auf,0,sizeof(b_data));
				auf.type = 432;
				strcpy(auf.ar[0].send_user,buf.send_user);
				strcpy(auf.ar[0].recv_user,rows[0]);
				strcpy(auf.ar[0].data,buf.recv_user);				//data存储群名称
				auf.ar[0].type = 32;
				auf.flag == 0;
				//判断对方是否离线
				if(Online(rows[0]) == 0)	{									//离线状态
					back_data->cnt = 1;				//搁置请求，等对方上线再发送
					New_offline(auf,0);
					return;
				}
				
				printf("send:%s recv:%s data:%s\n",auf.ar[0].send_user,auf.ar[0].recv_user,auf.ar[0].data);
				send(get_fd(auf.ar[0].recv_user),&auf,sizeof(b_data),0);	

			}
			//发送成功，反馈值置1
			back_data->cnt = 1;
		}
	}


	//若为第二次处理申请
	if(buf.flag == 1) {
		//在关系信息表中加入
		memset(query_str,0,strlen(query_str));
		printf("s-%s r-%s d-%s\n",buf.send_user,buf.recv_user,buf.data);
		sprintf(query_str,"insert into relationinfo(name1,name2,power) values('%s','%s',%d)",buf.send_user,buf.data,3);
		int rc = mysql_real_query(&mysql,query_str,strlen(query_str));
		if(rc != 0)
			printf("%s\n",mysql_error(&mysql));
		res = mysql_store_result(&mysql);
		
		
		b_data auf;
		memset(&auf,0,sizeof(auf));
		auf.type = 432;
		auf.flag = 1;
		strcpy(auf.ar[0].send_user,buf.send_user);
		strcpy(auf.ar[0].recv_user,buf.recv_user);
		
		//反馈给申请加入的用户
		//判断对方是否离线
		if(Online(buf.send_user) == 0)	{									//离线状态
			back_data->cnt = 1;				//搁置请求，等对方上线再发送
			New_offline(auf,0);
			return;
		}	

		send(get_fd(buf.send_user),&auf,sizeof(b_data),0);
		back_data->cnt = 1;
		return;
	}
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
	

	if(buf.flag == 0) {
		//判断双方是否为好友
		memset(query_str,0,strlen(query_str));
		sprintf(query_str,"select * from relationinfo where (name1='%s' and name2='%s') or (name1='%s' and name2='%s')",buf.send_user,buf.recv_user,buf.recv_user,buf.send_user);
		rc = mysql_real_query(&mysql,query_str,strlen(query_str));
		res = mysql_store_result(&mysql);
		row = mysql_num_rows(res);
		if(row != 0) {
			back_data->cnt = 0;					//说明双方已经是好友了
			return;	
		}
		//处理待发送的消息
		back_data->cnt = 1;

		b_data auf;
		memset(&auf,0,sizeof(b_data));
		auf.type = 431;
		strcpy(auf.ar[0].send_user,buf.send_user);
		strcpy(auf.ar[0].recv_user,buf.recv_user);
		auf.ar[0].type = 31;
		strcpy(auf.ar[0].data,buf.data);
		//判断对方是否离线
		if(Online(buf.recv_user) == 0)	{									//离线状态
			back_data->cnt = 1;				//搁置请求，等对方上线再发送
			New_offline(auf,0);
			return;
		}

		send(get_fd(buf.recv_user),&auf,sizeof(b_data),0);//发送请求
	}


	//若为第二次处理申请
	if(buf.flag == 1) {
		//在关系信息表中加入
		memset(query_str,0,strlen(query_str));
		sprintf(query_str,"insert into relationinfo(name1,name2,power) values('%s','%s',%d)",buf.send_user,buf.recv_user,4);
		mysql_real_query(&mysql,query_str,strlen(query_str));

		b_data auf;
		memset(&auf,0,sizeof(auf));
		auf.type = 431;
		auf.flag = 1;
		strcpy(auf.ar[0].send_user,buf.send_user);
		strcpy(auf.ar[0].send_user,buf.recv_user);
		
		//反馈给请求添加好友的用户
		//判断对方是否离线
		if(Online(buf.send_user) == 0)	{									//离线状态
			back_data->cnt = 1;				//搁置请求，等对方上线再发送
			New_offline(auf,0);
			return;
		}

		send(get_fd(buf.send_user),&auf,sizeof(b_data),0);
		return;
	}
	

}
//处理查看群成员请求
//参数;群名称
//反馈数据：返回该群聊的所有成员
void Show_member(request buf,b_data *back_data)
{
	MYSQL_RES	*res = NULL;
	MYSQL_ROW	rows;
	int row;
	char query_str[200];					//存放mysql语句的字符串数组
	int rc,i,fields;
	int count = 0;
	//在表中查找所有该群聊的成员

	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"select * from relationinfo where name2='%s'",buf.recv_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc != 0) {
		printf("error:%s",mysql_error(&mysql));
	}

	res = mysql_store_result(&mysql);
	row = mysql_num_rows(res);
	fields = mysql_num_fields(res);
	while(rows = mysql_fetch_row(res)) {
		puts(rows[0]);
		strcpy(back_data->str[count++],rows[0]);
	}
	back_data->cnt = 1;
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
	
	//判断该用户是否在群聊中
	memset(query_str,0,200);
	sprintf(query_str,"select * from relationinfo where name1='%s' and name2='%s'",buf.send_user,buf.recv_user);
	mysql_real_query(&mysql,query_str,strlen(query_str));
	puts("111");
	res = mysql_store_result(&mysql);
	puts("222");
	row = mysql_num_rows(res);
	
	printf("row = %d\n",row);
	if(row == 0) {				//该用户不在群聊中
		back_data->cnt = 0;
		return;
	}
	
	
	
	
	
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
	back_data->cnt = 1;

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

	//判断双方是否为好友
	memset(query_str,0,200);
	sprintf(query_str,"select * from relationinfo where (name1='%s' and name2='%s') or (name1='%s' and name2='%s')",buf.send_user,buf.recv_user,buf.recv_user,buf.send_user);
	mysql_real_query(&mysql,query_str,strlen(query_str));
	res = mysql_store_result(&mysql);
	row = mysql_num_rows(res);
	
	if(row == 0) {				//不是好友
		back_data->cnt = 0;
		return;
	}

	//在表中查找所有与该用户和该好友有关的聊天记录

	char *name1 = buf.send_user;
	char *name2 = buf.recv_user;
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"select * from recordinfo where send_user='%s' or recv_user='%s'",name1,name1);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	res = mysql_store_result(&mysql);
	row = mysql_num_rows(res);
	printf("row = %d\n",row);
	if(row == 0) {
		back_data->cnt = 1;
		return;
	}

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
	back_data->cnt = 1;
}

//显示某个用户是否在线
//参数：用户名
//若在线，返回1，若不在线，返回2
int Online(char *name)
{
	//根据用户名得到状态
	MYSQL_RES *res = NULL;
	MYSQL_ROW rows;
	char query_str[200];
	memset(query_str,0,200);
	sprintf(query_str,"select * from userinfo where name='%s'",name);
	mysql_real_query(&mysql,query_str,strlen(query_str));
	res = mysql_store_result(&mysql);
	int num = mysql_num_rows(res);
	if(num != 0) {
		rows = mysql_fetch_row(res);
		return rows[3][0] - '0';
	}
	return 0;
}




//处理显示所有群请求
//参数：用户名
//反馈数据:返回所有该用户的群信息，存入back_data中
void display_allg(request buf,b_data *back_data)
{
	MYSQL_RES	*res = NULL;
	MYSQL_ROW	rows;
	int row;
	char query_str[200];					//存放mysql语句的字符串数组
	int rc,i,fields;
	int count = 0;
	//在表中查找所有该用户所加入的群
	

	char *name = buf.send_user;
	int fd = buf.fd;
	
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"select * from relationinfo where name1='%s' and power !=4",buf.send_user);
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
				strcpy(back_data->str[count++],rows[1]);
	}
	
}


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
	sprintf(query_str,"select * from relationinfo where name1='%s' or name2='%s'",buf.send_user,buf.send_user);
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	res = mysql_store_result(&mysql);
	
	row = mysql_num_rows(res);
	if(row == 0) {
		return;
	}


	while((rows = mysql_fetch_row(res))) {
			if(strcmp(rows[0],name) == 0 && strcmp(rows[2],"4") == 0) {
				strcpy(back_data->str[count],rows[1]);
				back_data->str[count][strlen(rows[1])] = '\0';
				if(Online(rows[1]) == 1)
					strcat(back_data->str[count++],"(online)");
				else
					strcat(back_data->str[count++],"(offline)");
			}
			if(strcmp(rows[1],name) == 0 && strcmp(rows[2],"4") == 0)  {
				strcpy(back_data->str[count],rows[0]);
				back_data->str[count][strlen(rows[0])] = '\0';
				if(Online(rows[0]) == 1)
					strcat(back_data->str[count++],"(online)");
				else
					strcat(back_data->str[count++],"(offline)");
			}
			
	
	}
	
}
			

//处理登录请求
//参数：用户名name 密码 passwd
//反馈数据：在用户文件里面找到则返回int 1，否则返回0
int login_server(request buf)
{
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
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
	printf("rows = %d\n",rows);
		
	//判断该用户是否在线
	if(Online(buf.send_user) == 1) {
		printf("该用户已在线！！！\n");
		return 0;
	}
	if(rows != 0) {
		feedback = 1;
		//登录成功，将用户信息表中用户在线状态改为在线,套接字改为此次登录的所使用的套接字
		memset(query_str,0,200);
		sprintf(query_str,"update userinfo set state=%d,fd=%d where name='%s'",1,fd,name);

		rc = mysql_real_query(&mysql,query_str,strlen(query_str));
		if(rc != 0) {
			printf("%s",mysql_error(&mysql));
			printf("登录写入失败\n");
		}
		
	}




	return feedback;	
}

//处理注册请求
//参数：新用户名name新密码 passwd
//反馈数据：注册成功返回1，否则返回0
int register_server(request buf)
{
	MYSQL_RES *res = NULL;
	char query_str[200];
	int feedback = 0;							//反馈类型
	//在文件中查找是否存在该用户名
	int rc;
	
	int fd = buf.fd;
	memset(query_str,0,strlen(query_str));
	sprintf(query_str,"select * from userinfo where name='%s'",buf.send_user);
	mysql_real_query(&mysql,query_str,strlen(query_str));
	
	res = mysql_store_result(&mysql);
	int num = mysql_num_rows(res);
	printf("num = %d\n",num);	
	
	memset(query_str,0,strlen(query_str));
	
	
	
	if(num == 0) {
		feedback = 1;
		//注册成功，将新用户信息写入用户信息表
		sprintf(query_str,"insert into userinfo values('%s','%s',%d,%d)",buf.send_user,buf.data,fd,0);
		rc = mysql_real_query(&mysql,query_str,strlen(query_str));

		if(rc != 0) {
			feedback = 0;
			printf("注册写入失败\n");
		}
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
	int ret = 0;
	int length = 0;
	while(1) {
		length= recv(conn_fd,((char *)&buf + ret),sizeof(buf) - ret,0);
		if(length == 0 || length == -1) {
			MYSQL_RES *res = NULL;
			MYSQL_ROW	rows;
			char query_str[200];

			//改变用户信息表中此用户状态
			memset(query_str,0,200);
			sprintf(query_str,"update userinfo set state=0   where fd=%d",conn_fd);
			int rc = mysql_real_query(&mysql,query_str,strlen(query_str));
			if(rc != 0)
				printf(" %s\n",mysql_error(&mysql));
			pthread_exit(0);
		}
			

		ret+=length;

		if(ret == sizeof(buf)) {
			ret = 0;
			switch(buf.type) {
				case 0100:
					//处理登录请求
					printf("login:send_user = %s passwd = %s\n",buf.send_user,buf.data);
					memset(&back_data,0,sizeof(b_data));
					buf.fd = conn_fd;
					back_data.cnt = login_server(buf);
					send(conn_fd,&back_data,sizeof(b_data),0);
					break;
				case 0500:
					//处理显示离线请求
					printf("offline:%s --\n",buf.send_user);
					memset(&back_data,0,sizeof(b_data));
					back_data.ar[0].flag = 1;
					strcpy(back_data.ar[0].send_user,buf.send_user);
					
					New_offline(back_data,conn_fd);
					break;
				case 0110:
					//处理注册请求
					printf("%s %s\n",buf.send_user,buf.data);
					memset(&back_data,0,sizeof(b_data));
					back_data.cnt = register_server(buf);
					send(conn_fd,&back_data,sizeof(b_data),0);
					break;
				case 0130:
					//处理注销请求
					printf("Quit:%s \n",buf.send_user);
					Quit(buf);
					break;
				case 0240:
					//处理显示所有好友请求
					printf("display_allfriend:%s\n",buf.send_user);
					memset(&back_data,0,sizeof(b_data));
					display_all(buf,&back_data);
					send(conn_fd,&back_data,sizeof(b_data),0);
					break;
				case 0350:
					//处理显示所有群请求
					printf("display_allgroup%s\n",buf.send_user);
					memset(&back_data,0,sizeof(b_data));
					display_allg(buf,&back_data);
					send(conn_fd,&back_data,sizeof(b_data),0);
					break;
				case 0250:
					//处理查看私人聊天记录请求
					printf("View_record%s %s\n",buf.send_user,buf.recv_user);
					memset(&back_data,0,sizeof(b_data));
					View_record(buf,&back_data);
					send(conn_fd,&back_data,sizeof(b_data),0);
					break;
				case 0370:
					//处理查看群聊天记录请求
					printf("View_grecord%s %s\n",buf.send_user,buf.recv_user);
					memset(&back_data,0,sizeof(b_data));
					View_grecord(buf,&back_data);
					send(conn_fd,&back_data,sizeof(b_data),0);
					break;
				case 0360:
					//处理查看群成员请求
					printf("Show_member%s %s\n",buf.send_user,buf.recv_user);
					memset(&back_data,0,sizeof(b_data));
					Show_member(buf,&back_data);
					send(conn_fd,&back_data,sizeof(b_data),0);
					break;
				case 0310:
					//处理创建群聊请求
					printf("Create_group%s %s\n",buf.send_user,buf.data);
					memset(&back_data,0,sizeof(b_data));
					Create_Group(buf,&back_data);
					send(conn_fd,&back_data,sizeof(b_data),0);
					break;
				case 0210:
					//处理添加好友请求
					printf("Add_friend %s %s %d\n",buf.send_user,buf.recv_user,buf.flag);
					memset(&back_data,0,sizeof(b_data));
					Add_Friend(buf,&back_data);
					if(buf.flag == 0) {
						puts("haha");
						send(conn_fd,&back_data,sizeof(b_data),0);
					}
					break;
				case 0330:
					//处理添加群请求
					printf("Add_group%s %s %d %s\n",buf.send_user,buf.recv_user,buf.flag,buf.data);
					memset(&back_data,0,sizeof(b_data));
					Add_Group(buf,&back_data);
					send(conn_fd,&back_data,sizeof(b_data),0);
					break;
				case 0220:
					//处理删除好友请求
					printf("Delete_friend%s %s\n",buf.send_user,buf.recv_user);
					memset(&back_data,0,sizeof(b_data));
					Delete_Friend(buf,&back_data);
					send(conn_fd,&back_data,sizeof(b_data),0);
					break;
				case 0340:
					//处理退出群聊请求
					printf("Quit_Group%s %s\n",buf.send_user,buf.recv_user);
					memset(&back_data,0,sizeof(b_data));
					Quit_Group(buf,&back_data);
					send(conn_fd,&back_data,sizeof(b_data),0);
					break;
				case 0320:
					//处理解散群
					printf("Delete_Group%s %s\n",buf.send_user,buf.recv_user);
					memset(&back_data,0,sizeof(b_data));
					Delete_Group(buf,&back_data);
					send(conn_fd,&back_data,sizeof(b_data),0);
					break;

				case 0200:
					//处理发送消息请求
					printf("Send_message:%s %s %s\n",buf.send_user,buf.recv_user,buf.data);
					memset(&back_data,0,sizeof(b_data));
					Send_message(buf,&back_data);
					send(conn_fd,&back_data,sizeof(b_data),0);
					break;
				case 0300:
					//处理发送群聊消息请求
					printf("Send_gmessage:%s  %s\n",buf.send_user,buf.recv_user);
					memset(&back_data,0,sizeof(b_data));
					Send_gmessage(buf,&back_data);
					send(conn_fd,&back_data,sizeof(b_data),0);
					break;
				case 0400:
					//处理发送文件请求
					printf("Send_file recv:%s fl:%d data:%s size:%ld\n",buf.recv_user,buf.flag,buf.data,buf.size);
					memset(&back_data,0,sizeof(b_data));
					Send_file(buf,&back_data);
					break;
			/*	case 0372:
					//处理踢人请求
					printf("%s %s\n",buf.send_user,buf.recv_user);
					memset(&back_data,0,sizeof(b_data));
					Kick_member(buf,&back_data);
					send(conn_fd,&back_data,sizeof(b_data),0);
					break;
*/
				default:
					//
					break;
			}
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

	int num = 1;
	int length = sizeof(num);
	setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&num,length);

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

