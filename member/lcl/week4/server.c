/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月17日 星期五 14时35分06秒
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<unistd.h>
#include<mysql/mysql.h>
#include<sys/types.h>

#define PORT 8787
#define MAXSIZE 1024
#define LISTENQ 5
#define FDSIZE 1000
#define EPOLLEVENTS 100
#define IPADDRESS "128.0.0.102"


//函数声明
//创建套接字并进行绑定
int socket_bind(char *ip,int port);
//错误处理函数
void my_err(const char *err_string,int line);
//IO多路复用epoll
void do_epoll(int listenfd);
//添加监听描述符事件
void add_event(int epollfd,int fd,int state);
//事件处理函数
void handle_events(int epollfd,struct epoll_event *events,int num,int listenfd);
//处理接收到的连接
void handle_accept(int epollfd,int listenfd);
void mysql();


//flag的选择：
//1.登录  2.注册  3.添加好友  4.删除好友  5.显示好友列表  6.查询好友  7.私聊  8.查聊天记录  9.创建群
//10.解散群  11退出群  12.查找群  13.显示群列表  14.显示群成员  15.群聊  16.离线消息

struct use 
{
    int flag;//你的选择
    int back;//返回值
    char uname[30];//名字
    char addname[30];//添加的名字
    char delname[30];//删除的名字
    char upassword[30];//密码
    char message[MAXSIZE];//发送的东西
};



//错误处理函数
void my_err(const char *err_string,int line)
{
    fprintf(stderr,"line:%d ",line);
    perror(err_string);
    exit(1);
}

int main(int argc,char *argv[])
{
    //定义一个套接字
    int listenfd;
    //创建套接字并进行绑定
    listenfd=socket_bind(IPADDRESS,PORT);
    //监听
    listen(listenfd,LISTENQ);
    //IO多路复用epoll
    do_epoll(listenfd);
    return 0;
}


//IO多路复用epoll
void do_epoll(int listenfd)
{
    //定义一个epoll句柄
    int epollfd;
    //定义epoll中的结构体
    struct epoll_event events[EPOLLEVENTS];
    //定义需要等待的线程的数量
    int ret;
    //创建一个句柄
    epollfd=epoll_create(FDSIZE);
    //添加监听描述符事件
    add_event(epollfd,listenfd,EPOLLIN);
    for( ; ; )
    {
        //获取已经准备好的描述符事件
        ret=epoll_wait(epollfd,events,EPOLLEVENTS,-1);
       // printf("----%d\n",ret);
        //事件处理函数
        handle_events(epollfd,events,ret,listenfd);
    }
    //关闭句柄
    close(epollfd);
}


//事件处理函数
void handle_events(int epollfd,struct epoll_event *events,int num,int listenfd)
{
    printf("handle\n");
    struct use *USER;
    //循环时候要用的
    int i;
    //定义一个fd
    int fd;
    //进行遍历
    for(i=0;i<num;i++)
    {
        fd=events[i].data.fd;
        //根据不同来进行处理
        if((fd==listenfd)&&(events[i].events&EPOLLIN))
        {
            //printf("这时新的李恩杰\n");
            handle_accept(epollfd,listenfd);
        }
        else if(events[i].events&EPOLLIN)
        {
            printf("这时一个消息\n");
            if(recv(fd,(void *)USER,sizeof(struct use),0)==-1)
            {
                my_err("recv",__LINE__);
            }
            //控制
            //多线程
             
            switch(USER->flag)
            {
                case 1://登录
                    {
                        mysql(&USER,fd);
                        //去数据库查找账号密码是否正确
                        break;
                    }
                case 2://注册
                    {
                        //将传过来的账号密码存入表中
                        break;
                    }
                case 3://添加好友
                    {
                        //每个账号在数据库中有一张表，存的是他的好友关系，显示在不在线，组关系...
                        break;
                    }
                case 4://删除好友
                    {
                        //找这个账号下数据库中的表，在好友列表中删除这个好友
                        break;
                    }
                case 5://显示好友列表
                    {
                        //用链表将数据库中的内容读出来，然后再显示出来 
                        break;
                    }
                case 6://查询好友
                    {
                        //进入这个账号的数据空中的好友库中，查找是否有这个好友
                        break;
                    }
                case 7://私聊
                    {
                        //socket
                        break;
                    }
                case 8://查聊天记录
                    {
                        //表
                        break;
                    }
                case 9://创建群
                    {
                        break;
                    }
                case 10://解散群
                    {
                        break;
                    }
                case 11://退出群
                    {
                        break;
                    }
                case 12://查找群
                    {
                        break;
                    }
                case 13://显示群列表
                    {
                        break;
                    }
                case 14://显示群成员
                    {
                        break;
                    }
                case 15://群聊
                    {
                        break;
                    }
                case 16://离线消息
                    {
                        break;
                    }
            }
        }
    }
}


//处理接收到的连接
void handle_accept(int epollfd,int listenfd)
{
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen;
    if((clifd=accept(listenfd,(struct sockaddr *)&cliaddr,&cliaddrlen))==-1)
    {
        my_err("accept",__LINE__);
    }
    else
    {
        printf("accept a new client:%s:%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);
        //添加一个客户描述符和事件
        add_event(epollfd,clifd,EPOLLIN);
    }
}


//添加监听描述符事件
void add_event(int epollfd,int fd,int state)
{
    //定义一个套接字结构体类型
    struct epoll_event ev;
    ev.events=state;
    ev.data.fd=fd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
}


//创建套接字并进行绑定
int socket_bind(char *ip,int port)
{
    //定义一个套接字
    int listenfd;
    //定义套接字结构体地址
    struct sockaddr_in servaddr;
    //创建一个套接字
    if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        my_err("socket",__LINE__);
    }
    bzero(&servaddr,sizeof(servaddr));//清0
    //设置地址
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(port);
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    //绑定端口
    if(bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr))==-1)
    {
        my_err("bind",__LINE__);
    }
    return listenfd;
}


//数据库的操作
void mysql(struct use *USER,int fd)
{
    int num;
    MYSQL mysql;
    char sql[200];
    //初始化句柄
    if(NULL==mysql_init(&mysql))
    {
        printf("init:   %s\n",mysql_error(&mysql));
    }
    //建立连接
    if(mysql_real_connect(&mysql,"localhost","debian-sys-maint","5FBt2AwBpXoUKYdV","userlog",0,NULL,0))
    {
        printf("connect:  %s\n",mysql_error(&mysql));
    }
    //执行插入语句
    switch(USER->flag)
    {
        case 1://登录   查询mysql
            {
                memset(sql,0,sizeof(char)*200);
                strcpy(sql,"SELECT username,userpassword FROM userlogin");
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))//执行插入语句
                {
                    printf("query: %s\n",mysql_error(&mysql));
                }
                MYSQL_RES *res=mysql_store_result(&mysql);
                MYSQL_ROW row;
                while((row==mysql_fetch_row(res)))
                {
                    if((strcmp(row[0],USER->uname)==0)&&(strcmp(row[1],USER->upassword)==0))//如果输入账号密码正确
                    {
                        num=1;
                        if(send(fd,num,sizeof(int),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                        usleep(10000);
                            break;
                    }
                }
                num=0;//账号密码错误
                if(send(fd,num,sizeof(int),0)==-1)
                {
                    my_err("send",__LINE__);
                }
                break;
            }
        case 2://注册   添加mysql
            {
                memset(sql,0,sizeof(char)*200);
                sprintf(sql,"INSERT INTO userlogin(username,userpassword)VALUES('%s','%s');",USER->uname,USER->upassword);
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query: %s\n",mysql_error(&mysql));
                }
                else
                {
                    printf("\t\t添加成功！\n");
                }
                break;
            }
        case 3://添加好友  添加mysql
            {
                memset(sql,0,sizeof(char)*200);
                break;
            }
        case 4://删除好友
            {
                memset(sql,0,sizeof(char)*200);
                break;
            }
        case 5://显示好友列表   用一个全局的结构体
            {
                memset(sql,0,sizeof(char)*200);
                break;
            }
        case 6://查询好友
            {
                memset(sql,0,sizeof(char)*200);
                break;
            }
        case 8://查聊天记录
            {
                memset(sql,0,sizeof(char)*200);
                break;
            }
        case 9://创建群
            {
                memset(sql,0,sizeof(char)*200);
                break;
            }
        case 10://解散群
            {
                memset(sql,0,sizeof(char)*200);
                break;
            }
        case 11://退出群
            {
                memset(sql,0,sizeof(char)*200);
                break;
            }
        case 12://查找群
            {
                memset(sql,0,sizeof(char)*200);
                break;
            }
        case 13://显示群列表
            {
                memset(sql,0,sizeof(char)*200);
                break;
            }
        case 14://显示群成员
            {
                memset(sql,0,sizeof(char)*200);
                break;
            }
        case 16://离线消息
            {
                memset(sql,0,sizeof(char)*200);
                break;
            }
    }
}
//flag的选择：
//1.登录  2.注册  3.添加好友  4.删除好友  5.显示好友列表  6.查询好友  7.私聊  8.查聊天记录  9.创建群
//10.解散群  11退出群  12.查找群  13.显示群列表  14.显示群成员  15.群聊  16.离线消息

