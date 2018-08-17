#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<pthread.h>
#include<mysql/mysql.h>
#include<sys/time.h>
#include<sys/epoll.h>
/*
char sql_insert[200];
sprintf(sql_insert, "INSERT INTO table values('%s','%d');", name, age);
*/

/*服务端*/
#define PORT 8848
#define send_length 200

MYSQL *con;         //mysql连接
MYSQL_RES *res = NULL;  //mysql记录集
MYSQL_ROW row;      //字符串数组，mysql记录行
char *user = "debian-sys-maint";                                            
char *key = "NHqLsqHfc2mBjaKt";
char *host = "localhost";
char *db = "user";

void pool_add_worker(void(*process)(char *str, int socket), char *str, int socket);
void *thread_routine(void *arg);
void pool_init(int max_thread_num);
int pool_destroy();
void process(char *str, int socket);
void my_err(char *str, int line);
void user_login(char *str, int socket); 
void user_enter(char *str, int socket);
void find_passwd(char *str, int socket);
void rm_socket(int socket);
void send_message(char *str, int socket);
void add_friend(char *str, int socket);

/*任务结构*/
typedef struct worker
{
	void (*process)(char *str, int socket);
	char str[200];
	int socket;
	struct worker *next;
}create_worker;
/*线程池结构*/
typedef struct
{
	pthread_mutex_t queue_mutex;
	pthread_cond_t queue_cond;
	/*链表结构,线程池中有所等待任务*/
	create_worker *queue_head;
	/*是否销毁线程池*/
	int shutdown;
	pthread_t *threadid;
	/*线程池中允许的活动线程数目*/
	int max_thread_num;
	/*当前等待队列的任务数目*/
	int cur_queue_size;
}create_pool;


static create_pool *pool = NULL;
/*创建线程池*/
void pool_init(int max_thread_num)
{
	pool = (create_pool *)malloc(sizeof(create_pool));
	pthread_mutex_init(&(pool->queue_mutex), NULL);
	pthread_cond_init(&(pool->queue_cond), NULL);
	pool->queue_head = NULL;
	pool->max_thread_num = max_thread_num;
	pool->cur_queue_size = 0;
	pool->shutdown = 0;
	pool->threadid = (pthread_t *)malloc(sizeof(pthread_t) * max_thread_num);
	for(int i = 0; i < max_thread_num; i++)
	{
		pthread_create(&pool->threadid[i], NULL, thread_routine, NULL);
	}
}

/*错误处理函数*/
void my_err(char *str, int line)
{
	fprintf(stderr, "line: %d\n", line);
	perror(str);
	exit(-1);
}

/*向线程池中加入任务*/
void pool_add_worker(void (*process) (char *str, int socket), char *str, int socket)
{
	/*构建一个新任务*/
	create_worker *newworker = (create_worker*)malloc(sizeof(create_worker));
	newworker->process = process;
	strcpy(newworker->str, str);
	newworker->socket = socket;
	newworker->next = NULL;
	pthread_mutex_lock(&pool->queue_mutex);
	/*将任务加入到等待队列*/
	create_worker *member = pool->queue_head;
	if(member != NULL)
	{
		while(member->next != NULL)
			member = member->next;
		member->next = newworker;
	}
	else
		pool->queue_head = newworker;
	assert(pool->queue_head != NULL);
	pool->cur_queue_size++;
	pthread_mutex_unlock(&pool->queue_mutex);
	/*等待队列有任务，唤醒一个等待的线程*/
	pthread_cond_signal(&pool->queue_cond);
}
/*销毁线程池， 等待队列中的任务不会在执行，但是正在运行的线程一定会把任务运行完后在退出*/
int pool_destroy()
{
	if(pool->shutdown)
		return -1;		//防止两次调用	
	pool->shutdown = 1;
	/*唤醒所有的等待线程，线程池要销毁了*/
	pthread_cond_broadcast(&pool->queue_cond);

	/*阻塞等待线程退出，否则就成僵尸了*/
	for(int i = 0; i < pool->max_thread_num; i++)
		pthread_join(pool->threadid[i], NULL);      //等待所有线程执行完毕再销毁
	free(pool->threadid);
	/*销毁等待队列*/
	create_worker *head = NULL;
	while(pool->queue_head != NULL)
	{
		head = pool->queue_head;
		pool->queue_head = pool->queue_head->next;
		free(head);
	}

	/*销毁条件变量和互斥量*/
	pthread_mutex_destroy(&pool->queue_mutex);
	pthread_cond_destroy(&pool->queue_cond);
	free(pool);
	pool = NULL;
}

void *thread_routine(void *arg)
{
	//printf("线程开始运行 %ld\n", pthread_self());
	while(1)
	{
		pthread_mutex_lock(&pool->queue_mutex);
		/*如果等待队列为0并且不销毁线程池，则处于阻塞状态*/
		while(pool->cur_queue_size == 0 && !pool->shutdown)
		{
			printf("thread: %ld is wait\n", pthread_self());
			pthread_cond_wait(&pool->queue_cond, &pool->queue_mutex);
		}
	//	printf("shutdown is: %d\n", pool->shutdown);
		/*线程池要销毁了*/
		if(pool->shutdown)
		{
			/*遇到break, continue, return 等跳转语句，要记得先解锁*/
			pthread_mutex_unlock(&pool->queue_mutex);
			printf("thread %ld will exit\n", pthread_self());
			pthread_exit(NULL);
		}
		printf("thread %ld is starting to work\n", pthread_self());
		
		assert(pool->cur_queue_size != 0);
		assert(pool->queue_head != NULL);
		/*等待队列长度减１，并去除链表中的头元素*/
		pool->cur_queue_size--;
		create_worker *work = pool->queue_head;
		pool->queue_head = work->next;
		pthread_mutex_unlock(&pool->queue_mutex);

		/*调用回调函数，执行任务*/
		(work->process) (work->str, work->socket);
		free(work);
		work = NULL;
	}
	//这一句应该不可以到达执行
	pthread_exit(NULL);
}

/*回调函数*/
void process(char *str, int socket)
{
	printf("str = %s\n", str);
	printf("socket =  %d\n", socket);
	if(strncmp(str, "login", 5) == 0)
		user_login(str, socket);
	if(strncmp(str, "enter", 5) == 0)
		user_enter(str, socket);
	if(strncmp(str, "find_passwd", 11) == 0)
		find_passwd(str, socket);
	if(strncmp(str, "~", 1) == 0)
		send_message(str, socket);
}


/*解析用户注册的名字密码及密保问题*/
void user_login(char *str, int socket)
{
	char name[20] = {0};
	char passwd[20] = {0};
	char question[20] = {0};
	int i, j = 0, count = 0;
	for(i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == ':')
		{
			count++;
			i++;
			j = 0;
		}
		if(count == 1)               //将用户名解析出来
		{
			name[j++] = str[i];
			continue;
		}
		if(count == 2)
		{
			passwd[j++] = str[i];	//解析出密码
			continue;
		}
		if(count == 3)
			question[j++] = str[i];	//解析出回答
	}
	char sql_insert[100];
	sprintf(sql_insert, "INSERT INTO person(username, passwd, question) values('%s', '%s', '%s')",name, passwd, question);
	if(mysql_real_query(con, sql_insert, strlen(sql_insert)))
		my_err("INSERT", __LINE__);
	char flag[200] = "success";
	if(send(socket, flag, send_length, 0) < 0)
		my_err("send", __LINE__);
}

/*处理用户登录*/
void user_enter(char *str, int socket)
{
	char name[20] = {0};
	char passwd[20] = {0};
	int i, j = 0, count = 0;
	/*解析用户姓名及密码*/
	for(i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == ':')
		{
			count++;
			i++;
			j = 0;
		}
		if(count == 1)
			name[j++] = str[i];
		else if(count == 2)
			passwd[j++] = str[i];
	}
	/*将用户信息存储起来*/
	char *sql_select = "SELECT username, passwd from person";
	if(mysql_real_query(con, sql_select, strlen(sql_select)) )
		my_err("select", __LINE__);
	res = mysql_store_result(con);
	while(row = mysql_fetch_row(res))
	{
		/*登录成功*/
		if(strcmp(row[0], name) == 0 && strcmp(row[1], passwd) == 0)
		{
			/*登录成功后将套接字存入实时在线用户库中*/
			char sql_socket[150] = {0};
			sprintf(sql_socket, "INSERT INTO online(username, socket) values('%s', '%d')", row[0], socket);
			if( mysql_real_query(con, sql_socket, strlen(sql_socket)) )
				my_err("INSERT", __LINE__);
			char flag[200] = "success";
			if(send(socket, flag, send_length, 0) < 0)
				my_err("send", __LINE__);
			return;
		}
	}
	if(send(socket, "fault", 8, 0) < 0)
		my_err("send", __LINE__);

}

/*找回用户名*/
void find_passwd(char *str, int socket)
{
	char name[20] = {0};
	char question[20] = {0};
	int i, j = 0, count = 0;
	/*解析出问题和名字*/
	for(i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == ':')
		{
			count++;
			i++;
			j = 0;
		}
		if(count == 1)
			name[j++] = str[i];
		else if(count == 2)
			question[j++] = str[i];
	}
	printf("name: %s  question: %s\n", name, question);
	char *sql_select = "SELECT username, passwd, question from person";
	if(mysql_real_query(con, sql_select, strlen(sql_select)) )
		my_err("select", __LINE__);
	res = mysql_store_result(con);
	while(row = mysql_fetch_row(res))
	{
		if(strcmp(row[0], name) == 0 && strcmp(row[2], question) == 0)
		{
			char p[200] = {0};
			strcpy(p, row[1]);
			if(send(socket, p, 200, 0) < 0)
				my_err("send", __LINE__);
			return;
		}
	}
	if(send(socket,"fault", 6, 0) < 0)
		my_err("send", __LINE__);
	return;	
}

/*将离线用户的套接字删掉*/
void rm_socket(int socket)
{
	char sql_delete[150] = {0};
	sprintf(sql_delete, "DELETE FROM online where socket=%d", socket);
	if( mysql_real_query(con, sql_delete, strlen(sql_delete)) )
		my_err("sql_delete", __LINE__);
}

/*客户端之间发送消息*/
void send_message(char *str, int socket)
{
	char name[20] = {0};
	char send_message[200] = {0};
	char message[200] = {0};
	int i, j = 0, count = 0;
	int sock_fd;
	for(i = 1; ; i++)
	{
		if(str[i] == ':')
		{
			count++;
			i++;
		}
		if(count == 1)
		{
			strcpy(message, &str[i]);
			break;
		}
		else
			name[j++] = str[i];
	}
	char *sql_select = "SELECT username, socket from online";
	if( mysql_real_query(con, sql_select, strlen(sql_select)) )
		my_err("select", __LINE__);
	res = mysql_store_result(con);
	count = 0;
	printf("%s %s\n", name, message);
	while(row = mysql_fetch_row(res))
	{
		if(strcmp(name, row[0]) == 0)
		{
			sock_fd = atoi(row[1]);
			count++;
		}
		if(socket == atoi(row[1]))
		{
			strcpy(send_message, row[0]);
			send_message[strlen(send_message)] = ':';
			send_message[strlen(send_message)] = '\0';
			count++;
		}
		if(count == 2)
		{
			strcat(send_message, message);
			if(send(sock_fd, send_message, 200, 0) < 0)
				my_err("send", __LINE__);
		}
	}
	
}
/*添加好友*/
void add_friend(char *str, int socket)
{
	int sock_fd;
	char name[20] = {0};
	char message[200] = "add:";
	int i, j= 0, count = 0;
	for(i = 0; str[i] != '\0'; i++)
	{
		if(str[i] == ':')
			count++;
		if(count == 1)
			name[j++] = str[i];
	}
	printf("%s\n", name);
	char *sql_add = "SELECT username, socket FROM person";
	if( mysql_real_query(con, sql_add, strlen(sql_add)) )
		my_err("select", __LINE__);
	res = mysql_store_result(con);
	count = 0;
	while(row = mysql_fecht_row(res))
	{
		if(strcmp(name, row[0]) == 0)
		{
			sock_fd = row[1];
			count++;
		}
		if(socket == row[1])
		{
			strcat(message, row[0]);
			char *temp = " want to add you";
			strcat(message, temp);
			printf("add message = %s\n", message);
		}
		if(count == 2)
		{
			if(send(sock_fd, message, send_length, 0) < 0)
				my_err("send", __LINE__);
		}
	}
}

int main(void)
{
	/*初始化mysql*/
	con = mysql_init(con);
	mysql_real_connect(con, host, user, key, db, 0, NULL, 0);

	int serv_fd, cli_fd;
	int epfd;                  //epoll句柄
	epfd = epoll_create(256);
	struct sockaddr_in serv_addr, cli_addr; //服务端客户端套接字地址
	struct epoll_event serv_ev;             //epoll事件结构体
	int cond;								//epoll_wait 的返回值	
	char readbuf[200] = {0};            //读取客户端发来的消息
	int flag;                            //判断recv返回值
	if( (serv_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		my_err("socket", __LINE__);
	/*设置服务器结构*/
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	socklen_t len = sizeof(struct sockaddr);
	if(bind(serv_fd, (struct sockaddr *)&serv_addr, len) < 0)
		my_err("bind", __LINE__);
	if(listen(serv_fd, 20) < 0)
		my_err("listen", __LINE__);
	
	/*给epoll中的事件赋值*/
	struct epoll_event max_ev[64];
	/*监听服务器*/
	serv_ev.events = EPOLLIN;
	serv_ev.data.fd = serv_fd;
	if(epoll_ctl(epfd, EPOLL_CTL_ADD, serv_fd, &serv_ev) < 0)
		my_err("epoll_ctl", __LINE__);
	int i, fd;
	int recv_length = 200, count = 0; // 设置接收长度200， 与发送长度一致，防止数据流混乱
	pool_init(5);
	while(1)
	{	
		if( (cond = epoll_wait(epfd, max_ev, 64, -1)) < 0)
			my_err("epoll_wait", __LINE__);
		for(i = 0; i < cond; i++)
		{
			fd = max_ev[i].data.fd;
			/*接收用户登录*/
			if(fd == serv_fd)
			{
				if( (cli_fd = accept(serv_fd, (struct sockaddr *)&serv_addr, &len)) < 0)
					my_err("accept", __LINE__);
				printf("连接成功\n");
				serv_ev.data.fd = cli_fd;
				serv_ev.events = EPOLLIN;
				if(epoll_ctl(epfd, EPOLL_CTL_ADD, cli_fd, &serv_ev) < 0)
					my_err("epoll_ctl", __LINE__);
			}
			/*处理用户发送的消息*/
			else
			{
				while( (flag = recv(fd, readbuf, recv_length, 0)) )
				{
					printf("flag = %d\n", flag);
					count += flag;
					if(count == 200)
					 	break;
					else
						recv_length -= flag;
				}
				if(flag < 0)
					my_err("recv", __LINE__);
				printf("recv: %s\n", readbuf);
				/*recv 返回0 套接字断开连接，将其从句柄中删除*/
				if(flag == 0)
				{
					printf("%d用户断开连接\n", fd);
					rm_socket(fd);
					epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &max_ev[i]);
					close(fd);            //关闭文件描述符
				}

				else
				{
					pool_add_worker((process), readbuf, fd);
				}
			}
			recv_length = 200;
			count = 0;
			memset(readbuf, 0, sizeof(readbuf));
		}
	}
	close(serv_fd);
}
