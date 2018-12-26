/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月17日 星期五 14时35分06秒
 ************************************************************************/;
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<unistd.h>
#include<pthread.h>
#include<mysql/mysql.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define PORT 8686
#define MAXSIZE 1024
#define LISTENQ 5
#define FDSIZE 1000
#define EPOLLEVENTS 100
#define IPADDRESS "128.0.0.102"
int k=0;
struct use 
{
    int flag;//你的选择
    int flag2;
    int back;//回来的选择
    int fd;
    int sd;
    int size;
    char uname[30];//客户端当前登录名
    char gpname[30];//组名 
    char othername[30];//
    char upassword[30];//密码
    char message[MAXSIZE];//发送的东西
    char sendtime[50];
    char message_rece[MAXSIZE];//接收到东西
    char filename[40];
    char path[50];
};

//函数声明
void my_err(const char *err_string,int line);//错误处理函数
int socket_bind(char *ip,int port);//创建套接字并进行绑定
void do_epoll(int listenfd);//IO多路复用epoll
void add_event(int epollfd,int fd,int state);//添加监听描述符事件
void handle_accept(int epollfd,int listenfd);//处理接收到的连接
void thread();//父线程
void handle_events(int epollfd,struct epoll_event *events,int num,int listenfd);//事件处理函数
void mysql();//数据库的操作

//错误处理函数
void my_err(const char *err_string,int line)
{
    fprintf(stderr,"line:%d ",line);
    perror(err_string);
    exit(1);
}

//主函数
int main(int argc,char *argv[])
{
    int listenfd;
    listenfd=socket_bind(IPADDRESS,PORT);   
    listen(listenfd,LISTENQ);
    do_epoll(listenfd);
    return 0;
}

//创建套接字并进行绑定
int socket_bind(char *ip,int port)
{
    int listenfd;
    struct sockaddr_in servaddr;
    if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        my_err("socket",__LINE__);
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(port);
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    if(bind(listenfd,(struct sockaddr *)&servaddr,sizeof(struct sockaddr_in))==-1)
    {
        my_err("bind",__LINE__);
    }
    return listenfd;
}

//IO多路复用epoll
void do_epoll(int listenfd)
{
    int epollfd;
    struct epoll_event events[EPOLLEVENTS];
    int ret;
    epollfd=epoll_create(FDSIZE);
    add_event(epollfd,listenfd,EPOLLIN);
    for( ; ; )
    {
        ret=epoll_wait(epollfd,events,EPOLLEVENTS,-1);
        handle_events(epollfd,events,ret,listenfd);
    }
    close(epollfd);
}

//添加监听描述符事件
void add_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events=state;
    ev.data.fd=fd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
}

//事件处理函数
void handle_events(int epollfd,struct epoll_event *events,int num,int listenfd)
{
    struct use USER;
    int fd;
    pthread_t thid;
    int i;
    memset(&USER,0,sizeof(struct use));
    for(i=0;i<num;i++)
    {
        fd=events[i].data.fd;
        if(fd==listenfd)
        {
            handle_accept(epollfd,listenfd);
        }
        else if(events[i].events&EPOLLIN)
        {
            if((recv(fd,&USER,sizeof(struct use),0)<=0))
            {
                //连接断开，关闭连接
                memset(&USER,0,sizeof(struct use));
                USER.flag=20;
                mysql(USER,fd);
                close(fd);
                continue;
            }
            USER.fd=fd;
            if(pthread_create(&thid,NULL,(void *)thread,(void *)&USER)!=0)
            {
                my_err("pthread_create",__LINE__);
            }
        }
    }
}

//处理接收到的连接
void handle_accept(int epollfd,int listenfd)
{
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen=sizeof(struct sockaddr_in);
    if((clifd=accept(listenfd,(struct sockaddr *)&cliaddr,&cliaddrlen))==-1)
    {
        my_err("accept",__LINE__);
    }
    else
    {
        printf("accept a new client:%s:%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);
        add_event(epollfd,clifd,EPOLLIN);
    }
}

void thread(void *SE)
{
    struct use USER=*(struct use *)SE;
    switch(USER.flag)
            {
                case 1://登录
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 2://注册
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 3://添加好友
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 4://删除好友
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 5://显示好友列表
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 6://查询好友
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 7://私聊
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 8://查聊天记录
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 9://创建群
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 10://解散群
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 11://退出群
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 12://查看群聊天记录
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 13://显示群列表
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 14://显示群成员
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 15://群聊
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 16://添加群
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 17://查看离线消息 添加
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 18://查看离线消息 消息
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 19://接受请求的信息
                    {
                        mysql(USER,USER.fd);
                        break;
                    }
                case 21:
                    {
                        mysql(USER,USER.fd);
                        break;
                    }          
            }
}

//数据库的操作
void mysql(struct use USER,int fd)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    MYSQL mysql;
    char M_id[4],O_id[4];
    int MM[30];
    char my_other[30],other_my[30];
    char ptr[10];//发送的yes/no消息
    char sql[200];//mysql执行语句
    int flag1=0,flag2=0;
    int my_id,other_id;
    int other_fd;
    int my_on_out;
    char my_name[100],other_name[100];
    char my_friends[100],other_friends[100];
    char send_mation[100],send_time_mation[150];
    char g_users[100];
    char other_groups[100],my_groups[100];
    int g_id,other_gid;
    int i=0,j=0;
    char send1[200];//发送的消息
    if(NULL==mysql_init(&mysql))
    {
        printf("init:   %s\n",mysql_error(&mysql));
    }
    if(!mysql_real_connect(&mysql,"localhost","debian-sys-maint","5FBt2AwBpXoUKYdV","userlog",0,NULL,0))
    {
        printf("connect:  %s\n",mysql_error(&mysql));
    }
    switch(USER.flag)
    {
        case 1://登录
            {
                flag1=0;
                memset(sql,0,sizeof(char)*200);
                strcpy(sql,"SELECT username,userpassword FROM userlogin;");
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                while((row=mysql_fetch_row(res)))
                {
                    if(strcmp(row[0],USER.uname)==0)
                    {
                        if(strcmp(row[1],USER.upassword)==0)
                        {
                            flag1=1;
                            strcpy(USER.message_rece,"yes");
                            memset(sql,0,sizeof(char)*200);
                            sprintf(sql,"UPDATE userlogin SET sockfd='%d',on_out='1'WHERE username='%s';",fd,USER.uname);
                            if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                            {
                                printf("query:%s\n",mysql_error(&mysql));
                            }
                            if(send(fd,&USER,sizeof(USER),0)==-1)
                            {
                                my_err("send",__LINE__);
                            }
                            break;
                        }
                        else
                        {
                            flag1=2;
                            strcpy(USER.message_rece,"ns");//密码输入错误
                            if(send(fd,&USER,sizeof(USER),0)==-1)
                            {
                                my_err("send",__LINE__);
                            }
                            break;
                        }
                    }
                }
                mysql_free_result(res);
                if(flag1==0)
                {
                    strcpy(USER.message_rece,"no");//没有这个用户名
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                    break;
                }
                else
                {
                    break;
                }
                break;
            }
        case 2://注册
            {
                memset(ptr,0,sizeof(ptr));
                flag1=0;
                strcpy(sql,"SELECT username FROM userlogin;");
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                while((row=mysql_fetch_row(res)))
                {
                    if(strcmp(row[0],USER.uname)==0)
                    {
                        flag1=1;
                        strcpy(USER.message_rece,"no");//用户名被注册过
                        if(send(fd,&USER,sizeof(USER),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                        break;
                    }
                }
                mysql_free_result(res);
                if(flag1==1)
                    break;
                else
                {
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"INSERT INTO userlogin(username,userpassword,sockfd,on_out,friends,groups)VALUES('%s','%s',0,0,'00','00');",USER.uname,USER.upassword);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    strcpy(USER.message_rece,"yes");
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                break;
            }
        case 3://添加好友
            {
                flag1=0;
                USER.back=1;
                memset(my_friends,0,sizeof(my_friends));
                memset(send_mation,0,sizeof(send_mation));
                memset(sql,0,sizeof(sql));

                strcpy(sql,"SELECT username,id FROM userlogin;");

                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }

                res=mysql_store_result(&mysql);
                while((row=mysql_fetch_row(res)))
                {
                    if(strcmp(row[0],USER.othername)==0)
                    {
                        flag1=1;
                        other_id=atoi(row[1]);
                        break;
                    }
                }
                mysql_free_result(res);
                if(flag1==0)
                {
                    strcpy(USER.message_rece,"no");
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                else
                {
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"SELECT friends FROM userlogin WHERE username='%s';",USER.uname);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    res=mysql_store_result(&mysql);
                    row=mysql_fetch_row(res);
                    strcpy(my_friends,row[0]);
                    mysql_free_result(res);
                    for(i=0;i<strlen(my_friends);i+=3)
                    {
                        memset(other_friends,0,sizeof(other_friends));
                        other_friends[0]=my_friends[i];
                        other_friends[1]=my_friends[i+1];
                        if(atoi(other_friends)==other_id)
                        {
                            flag1=3;//已经是好友了
                            break;
                        }
                    }
                    if(flag1==3)
                    {
                        strcpy(USER.message_rece,"ns");
                        if(send(fd,&USER,sizeof(USER),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                    else
                    {
                        memset(sql,0,sizeof(sql));
                        strcpy(send_mation,USER.message);
                        sprintf(sql,"INSERT INTO formationbox(s_n,r_n,c_mation,fg2)VALUES('%s','%s','%s',1);",USER.uname,USER.othername,send_mation);
                        if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                        {
                            printf("query:%s\n",mysql_error(&mysql));
                        }
                        strcpy(USER.message_rece,"yes");
                        if(send(fd,&USER,sizeof(USER),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                }
                break;
            }
        case 4://删除好友
            {
                flag1=0;
                memset(sql,0,sizeof(sql));
                memset(my_friends,0,sizeof(my_friends));
                memset(other_friends,0,sizeof(other_friends));
                sprintf(sql,"SELECT id,friends FROM userlogin WHERE username='%s';",USER.uname);
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                row=mysql_fetch_row(res);
                my_id=atoi(row[0]);
                strcpy(my_friends,row[1]);
                mysql_free_result(res);
                memset(sql,0,sizeof(sql));
                strcpy(sql,"SELECT username,id,friends FROM userlogin;");
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                while((row=mysql_fetch_row(res)))
                {
                    if(strcmp(USER.othername,row[0])==0)
                    {
                        flag1=1;
                        other_id=atoi(row[1]);
                        strcpy(other_friends,row[2]);
                        break;
                    }
                }
                mysql_free_result(res);
                if(flag1==0)//想删除的人不存在
                {
                    strcpy(USER.message_rece,"ns");
                    USER.back=2;
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                else
                {
                    for(i=0;i<strlen(my_friends);i+=3)
                    {
                        memset(my_other,0,sizeof(my_other));
                        my_other[0]=my_friends[i];
                        my_other[1]=my_friends[i+1];
                        if(atoi(my_other)==other_id)//这个之前确实是我的好朋友
                        {
                            my_friends[i]='0';
                            my_friends[i+1]='0';
                            for(j=0;j<strlen(other_friends);j+=3)
                            {
                                memset(other_my,0,sizeof(other_my));
                                other_my[0]=other_friends[j];
                                other_my[1]=other_friends[j+1];
                                if(atoi(other_my)==my_id)
                                {
                                    other_friends[j]='0';
                                    other_friends[j+1]='0';
                                    break;
                                }
                            }
                            memset(sql,0,sizeof(sql));
                            sprintf(sql,"UPDATE userlogin SET friends='%s'WHERE id='%d';",my_friends,my_id);
                            if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                            {
                                printf("query:%s\n",mysql_error(&mysql));
                            }
                            memset(sql,0,sizeof(sql));
                            sprintf(sql,"UPDATE userlogin SET friends='%s'WHERE id='%d';",other_friends,other_id);
                            if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                            {
                                printf("query:%s\n",mysql_error(&mysql));
                            }
                            strcpy(USER.message_rece,"yes");
                            USER.back=2;
                            if(send(fd,&USER,sizeof(USER),0)==-1)
                            {
                                my_err("send",__LINE__);
                            }
                            break;
                        }
                    }
                    if(i==strlen(my_friends)+2)//他之前不是你的好友
                    {
                        strcpy(USER.message_rece,"no");
                        USER.back=2;
                        if(send(fd,&USER,sizeof(USER),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                }
                break;
            }
        case 5://显示好友列表
            {
                flag1=0;
                memset(sql,0,sizeof(sql));
                memset(my_friends,0,sizeof(my_friends));
                sprintf(sql,"SELECT friends FROM userlogin WHERE username='%s';",USER.uname);
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                row=mysql_fetch_row(res);
                strcpy(my_friends,row[0]);
                mysql_free_result(res);
                USER.back=3;
                for(i=0;i<strlen(my_friends);i+=3)
                {
                    memset(my_other,0,sizeof(my_other));
                    my_other[0]=my_friends[i];
                    my_other[1]=my_friends[i+1];
                    flag1=atoi(my_other);
                    if(flag1==0)
                        continue;
                    else
                    {
                        memset(sql,0,sizeof(sql));
                        sprintf(sql,"SELECT username,on_out FROM userlogin WHERE id='%d';",flag1);
                        if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                        {
                            printf("query:%s\n",mysql_error(&mysql));
                        }
                        res=mysql_store_result(&mysql);
                        row=mysql_fetch_row(res);
                        strcat(USER.message_rece,"\t\t");
                        strcat(USER.message_rece,row[0]);
                        strcat(USER.message_rece,"\t\t");
                        if(atoi(row[1])==1)
                        {
                            strcat(USER.message_rece,"\t\t在线\n");
                        }
                        else
                        {
                            strcat(USER.message_rece,"\t\t离线\n");
                        }
                        mysql_free_result(res);
                    }
                }
                if(send(fd,&USER,sizeof(USER),0)==-1)
                {
                    my_err("send",__LINE__);
                }
                break;
            }
        case 6://查询好友
            {
                flag1=0;
                USER.back=4;
                memset(sql,0,sizeof(sql));
                memset(my_friends,0,sizeof(my_friends));
                sprintf(sql,"SELECT friends FROM userlogin WHERE username='%s';",USER.uname);
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                row=mysql_fetch_row(res);
                strcpy(my_friends,row[0]);
                mysql_free_result(res);
                memset(sql,0,sizeof(sql));
                strcpy(sql,"SELECT username,id FROM userlogin;");
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                while((row=mysql_fetch_row(res)))
                {
                    if((strcmp(USER.othername,row[0]))==0)
                    {
                        flag1=1;
                        other_id=atoi(row[1]);
                        break;
                    }
                }
                mysql_free_result(res);
                if(flag1==1)
                {
                    for(i=0;i<strlen(my_friends);i+=3)
                    {
                        memset(my_other,0,sizeof(my_other));
                        my_other[0]=my_friends[i];
                        my_other[1]=my_friends[i+1];
                        if(atoi(my_other)==other_id)//该好友存在
                        {
                            memset(sql,0,sizeof(sql));
                            sprintf(sql,"SELECT on_out FROM userlogin WHERE username='%s';",USER.othername);
                            if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                            {
                                printf("query:%s\n",mysql_error(&mysql));
                            }
                            res=mysql_store_result(&mysql);
                            row=mysql_fetch_row(res);
                            my_on_out=atoi(row[0]);
                            mysql_free_result(res);
                            if(my_on_out==1)
                            {
                                strcpy(USER.message_rece,"yes");
                                if(send(fd,&USER,sizeof(USER),0)==-1)
                                {
                                    my_err("send",__LINE__);
                                }
                                break;
                            }
                            else
                            {
                                strcpy(USER.message_rece,"no");
                                if(send(fd,&USER,sizeof(USER),0)==-1)
                                {
                                    my_err("send",__LINE__);
                                }
                                break;
                            }
                            mysql_free_result(res);
                        }
                    }
                    if(i==strlen(my_friends)+2)
                    {
                        strcpy(USER.message_rece,"ns");//这个用户不是你的好友
                        if(send(fd,&USER,sizeof(USER),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                }
                else
                {
                    strcpy(USER.message_rece,"os");//这个用户没有注册过
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                break;
            }
        case 7://私聊
            {
                USER.back=5;
                memset(sql,0,sizeof(sql));
                memset(send_mation,0,sizeof(send_mation));
                memset(send_time_mation,0,sizeof(send_time_mation));
                sprintf(sql,"SELECT sockfd,on_out FROM userlogin WHERE username='%s';",USER.othername);
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                row=mysql_fetch_row(res);
                other_id=atoi(row[0]);
                my_id=atoi(row[1]);
                mysql_free_result(res);
                strcpy(send_time_mation,USER.sendtime);
                send_time_mation[strlen(send_time_mation)]='\0';
                strcpy(send_mation,USER.message);
                send_mation[strlen(send_mation)]='\0';
                memset(sql,0,sizeof(sql));
                sprintf(sql,"INSERT INTO chat_friends(send_name,rece_name,chat_time,chat_mation)VALUES('%s','%s','%s','%s');",USER.uname,USER.othername,send_time_mation,send_mation);
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                if(my_id==1)
                {
                    strcat(USER.message_rece,send_time_mation);
                    strcat(USER.message_rece,": ");
                    strcpy(USER.message_rece,send_mation);
                    if(send(other_id,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                else
                {
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"INSERT INTO formationbox(s_n,r_n,c_time,c_mation,fg2)VALUES('%s','%s','%s','%s',3);",USER.uname,USER.othername,send_time_mation,send_mation);
                    if(0!=mysql_real_query(&mysql,sql,sizeof(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                }
                break;
            }
        case 8://查找聊天记录
            {
                flag1=0;
                USER.back=6;
                memset(sql,0,sizeof(sql));
                strcpy(sql,"SELECT send_name,rece_name,chat_time,chat_mation FROM chat_friends;");
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                while((row=mysql_fetch_row(res)))
                {
                    if(((strcmp(row[0],USER.uname)==0)&&(strcmp(row[1],USER.othername)==0))||((strcmp(row[0],USER.othername)==0)&&(strcmp(row[1],USER.uname)==0)))
                    {
                        flag1=1;
                        strcat(USER.message_rece,"\t\t");
                        strcat(USER.message_rece,row[0]);
                        strcat(USER.message_rece,":\n");
                        strcat(USER.message_rece,"\t\t");
                        strcat(USER.message_rece,row[2]);
                        strcat(USER.message_rece,":\n");
                        strcat(USER.message_rece,"\t\t");
                        strcat(USER.message_rece,row[3]);
                        strcat(USER.message_rece,"\n\n");
                    }
                }
                mysql_free_result(res);
                if(flag1==0)
                {
                    strcpy(USER.message_rece,"no");
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                else
                {
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                break;
            }
        case 9://创建群
            {
                flag1=0;
                USER.back=7;
                memset(sql,0,sizeof(sql));
                strcpy(sql,"SELECT group_name FROM groups;");
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                while(row=mysql_fetch_row(res))
                {
                    if(strcmp(row[0],USER.gpname)==0)
                    {
                        flag1=1;
                        break;
                    }
                }
                mysql_free_result(res);
                if(flag1==1)
                {
                    strcpy(USER.message_rece,"no");
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                else
                {
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"INSERT INTO groups(group_name,group_owner,group_users)VALUES('%s','%s','00');",USER.gpname,USER.uname);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    strcpy(USER.message_rece,"yes");
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                break;
            }
        case 10://解散群
            {
                flag1=0;
                USER.back=8;
                memset(my_friends,0,sizeof(my_friends));
                memset(other_friends,0,sizeof(other_friends));
                memset(sql,0,sizeof(sql));
                memset(g_users,0,sizeof(g_users));
                strcpy(sql,"SELECT group_name,group_id,group_users,group_owner FROM groups;");
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                while((row=mysql_fetch_row(res)))
                {
                    if(strcmp(row[0],USER.gpname)==0)
                    {
                        flag1=1;//不是群主
                        if(strcmp(row[3],USER.uname)==0)//群主
                        {
                            flag1=2;
                            strcpy(g_users,row[2]);
                            g_id=atoi(row[1]);
                            break;
                        }
                    }
                }
                mysql_free_result(res);
                if(flag1==0)//没有这个群
                {
                    strcpy(USER.message_rece,"ns");

                    if (send (fd, &USER, sizeof (USER), 0) == -1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                else if(flag1==1)//不是群主，不能解散
                {
                    strcpy(USER.message_rece,"no");

                    if (send (fd, &USER, sizeof(USER), 0) == -1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                else if(flag1==2)//可以解散了
                {
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"DELETE FROM groups WHERE group_name='%s';",USER.gpname);
                    if(mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    for(i=0;i<strlen(g_users);i+=3)
                    {
                        memset(sql,0,sizeof(sql));
                        my_friends[0]=g_users[i];
                        my_friends[1]=g_users[i+1];
                        other_gid =atoi(my_friends);
                        sprintf(sql,"SELECT groups FROM userlogin WHERE id='%d';",other_gid);
                        if(mysql_real_query(&mysql,sql,strlen(sql)))
                        {
                            printf("query:%s\n",mysql_error(&mysql));
                        }
                        res=mysql_store_result(&mysql);
                        row=mysql_fetch_row(res);
                        strcpy(other_groups,row[0]);
                        mysql_free_result(res);
                        for(j = 0; j < strlen(other_groups); j += 3)
                        {
                            other_friends[0]=other_groups[j];
                            other_friends[1]=other_groups[j+1];
                            if(atoi(other_friends)==g_id)
                            {
                                other_groups[j]='0';
                                other_groups[j+1]='0';
                                break;
                            }
                        }
                        memset(sql,0,sizeof(sql));
                        sprintf(sql,"UPDATE userlogin SET groups='%s'WHERE id='%d';",other_groups,other_gid);
                        if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                        {
                            printf("query:%s\n",mysql_error(&mysql));
                        }
                        memset(sql,0,sizeof(sql));
                    }
                    strcpy(USER.message_rece,"yes");
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                break;
            }
        /* 退出群 */
        case 11://退出群
            {
                flag1=0;
                USER.back=9;
                memset(sql,0,sizeof(sql));
                memset(my_other,0,sizeof(my_other));
                memset(other_my,0,sizeof(other_my));
                memset(other_groups,0,sizeof(other_groups));
                memset(g_users,0,sizeof(g_users));
                strcpy(sql,"SELECT group_name,group_id,group_users,group_owner FROM groups;");
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                while((row=mysql_fetch_row(res)))
                {
                    if(strcmp(row[0],USER.gpname)==0)
                    {
                        if(strcmp(row[3],USER.uname)==0)
                        {
                            flag1=1;//是群主，不能退
                            break;
                        }
                        else
                        {
                            flag1=2;
                            strcpy(g_users,row[2]);
                            g_id=atoi(row[1]);
                            break;
                        }
                    }
                }
                mysql_free_result(res);
                if(flag1==0)//没有这个群
                {
                    strcpy(USER.message_rece,"yo");
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                else if(flag1==1)//他是群主，不能退群
                {
                    strcpy(USER.message_rece,"ns");
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                else if(flag1==2)
                {
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"SELECT groups,id FROM userlogin WHERE username='%s';",USER.uname);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    res=mysql_store_result(&mysql);
                    row=mysql_fetch_row(res);
                    strcpy(other_groups,row[0]);
                    my_id=atoi(row[1]);
                    mysql_free_result(res);
                    for(i=0;i<strlen(other_groups);i+=3)
                    {
                        my_other[0]=other_groups[i];
                        my_other[1]=other_groups[i+1];
                        if(g_id==atoi(my_other))
                        {
                            flag1=3;
                            other_groups[i]='0';
                            other_groups[i+1]='0';
                            memset(sql,0,sizeof(sql));
                            sprintf(sql,"UPDATE userlogin SET groups='%s'WHERE username='%s';",other_groups,USER.uname);
                            if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                            {
                                printf("query:%s\n",mysql_error(&mysql));
                            }
                            break;
                        }
                    }
                    if(flag1==3)
                    {
                        for(i=0;i<strlen(g_users);i+=3)
                        {
                            other_my[0]=g_users[i];
                            other_my[1]=g_users[i+1];
                            if(atoi(other_my)==my_id)
                            {
                                g_users[i]='0';
                                g_users[i+1]='0';
                                break;
                            }
                        }
                        memset(sql,0,sizeof(sql));
                        sprintf(sql,"UPDATE groups SET group_users='%s'WHERE group_name='%s';",g_users,USER.gpname);
                        if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                        {
                            printf("query:%s\n",mysql_error(&mysql));
                        }
                        strcpy(USER.message_rece,"yes");
                        if(send(fd,&USER,sizeof(USER),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                    else
                    {
                        strcpy(USER.message_rece,"no");
                        if(send(fd,&USER,sizeof(USER),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                }
                break;
            }
        case 12://查看聊天记录
            {
                flag1=0;
                USER.back=10;
                memset(my_other,0,sizeof(my_other));
                memset(sql,0,sizeof(sql));
                memset(g_users,0,sizeof(g_users));
                strcpy(sql,"SELECT group_name,group_users FROM groups;");
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                while((row=mysql_fetch_row(res)))
                {
                    if(strcmp(row[0],USER.gpname)==0)
                    {
                        flag1=1;
                        strcpy(g_users,row[1]);
                        break;
                    }
                }
                mysql_free_result(res);
                if(flag1==0)//没有这个群
                {
                    strcpy(USER.message_rece,"ns");
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                else if(flag1==1)
                {
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"SELECT id FROM userlogin WHERE username='%s';",USER.uname);
                    if(0!=mysql_real_query(&mysql,sql,sizeof(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    res=mysql_store_result(&mysql);
                    row=mysql_fetch_row(res);
                    my_id=atoi(row[0]);
                    mysql_free_result(res);
                    for(i=0;i<strlen(g_users);i+=3)
                    {
                        my_other[0]=g_users[i];
                        my_other[1]=g_users[i+1];
                        if(atoi(my_other)==my_id)
                        {
                            flag1=2;
                            break;
                        }
                    }
                    if(flag1==2)
                    {
                        memset(sql,0,sizeof(sql));
                        strcpy(sql,"SELECT gr_name,sendg_name,cg_time,cg_mation FROM chat_groups;");
                        if(0!=mysql_real_query(&mysql,sql,sizeof(sql)))
                        {
                            printf("query:%s\n",mysql_error(&mysql));
                        }
                        res=mysql_store_result(&mysql);
                        while(row=mysql_fetch_row(res))
                        {
                            if(strcmp(row[0],USER.gpname)==0)
                            {
                                strcat(USER.message_rece,"\t\t");
                                strcat(USER.message_rece,row[1]);
                                strcat(USER.message_rece,":\n");
                                strcat(USER.message_rece,"\t\t");
                                strcat(USER.message_rece,row[2]);
                                strcat(USER.message_rece,":\n");
                                strcat(USER.message_rece,"\t\t");
                                strcat(USER.message_rece,row[3]);
                                strcat(USER.message_rece,":\n");
                            }
                        }
                        mysql_free_result(res);
                        if(send(fd,&USER,sizeof(USER),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                    else
                    {
                        strcpy(USER.message_rece,"no");
                        if(send(fd,&USER,sizeof(USER),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                }
                break;
            }
        case 13://显示群列表
            {
                USER.back=11;
                memset(sql,0,sizeof(sql));
                memset(my_other,0,sizeof(my_other));
                memset(other_groups,0,sizeof(other_groups));
                sprintf(sql,"SELECT groups FROM userlogin WHERE username='%s';",USER.uname);
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                row=mysql_fetch_row(res);
                strcpy(other_groups,row[0]);
                mysql_free_result(res);
                for(i=0;i<strlen(other_groups);i+=3)
                {
                    my_other[0]=other_groups[i];
                    my_other[1]=other_groups[i+1];
                    g_id=atoi(my_other);
                    if(g_id==0)
                        continue;
                    else
                    {
                        sprintf(sql,"SELECT group_name FROM groups WHERE group_id='%d';",g_id);
                        if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                        {
                            printf("%s\n",mysql_error(&mysql));
                        }
                        res=mysql_store_result(&mysql);
                        row=mysql_fetch_row(res);
                        j++;
                        strcat(USER.message_rece,"\t\t");
                        strcat(USER.message_rece,row[0]);
                        if(j%3==0)
                        {
                            strcat(USER.message_rece,"\n\n");
                        }
                        mysql_free_result(res);
                    }
                }
                strcat(USER.message_rece,"\n\n");
                if(send(fd,&USER,sizeof(USER),0)==-1)
                {
                    my_err("send",__LINE__);
                }
                break;
            }
        case 14://显示群成员
            {
                flag1=0;
                USER.back=12;
                memset(sql,0,sizeof(sql));
                memset(my_other,0,sizeof(my_other));
                memset(other_my,0,sizeof(other_my));     
                memset(g_users,0,sizeof(g_users));
                strcpy(sql,"SELECT group_name,group_users FROM groups;");
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                while((row=mysql_fetch_row(res)))
                {
                    if(strcmp(row[0],USER.gpname)==0)
                    {
                        flag1=1;
                        strcpy(g_users,row[1]);
                        break;
                    }
                }
                mysql_free_result(res);
                if(flag1==0)
                {
                    strcpy(USER.message_rece,"ns");
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                else if(flag1==1)
                {
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"SELECT id FROM userlogin WHERE username='%s';",USER.uname);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    res=mysql_store_result(&mysql);
                    row=mysql_fetch_row(res);
                    my_id=atoi(row[0]);
                    mysql_free_result(res);
                    for(i=0;i<strlen(g_users);i+=3)
                    {
                        my_other[0]=g_users[i];
                        my_other[1]=g_users[i+1];
                        if(atoi(my_other)==my_id)
                        {
                            flag1=2;
                            break;
                        }
                    }
                    if(flag1==2)
                    {
                        for(i=0;i<strlen(g_users);i+=3)
                        {
                            memset(sql,0,sizeof(sql));
                            other_my[0]=g_users[i];
                            other_my[1]=g_users[i+1];
                            other_id=atoi(other_my);
                            if(other_id==0)
                                continue;
                            else
                            {
                                sprintf(sql,"SELECT username FROM userlogin WHERE id='%d';",other_id);
                                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                                {
                                    printf("query:%s\n",mysql_error(&mysql));
                                }
                                res=mysql_store_result(&mysql);
                                row=mysql_fetch_row(res);
                                j++;
                                strcat(USER.message_rece,"\t\t");
                                strcat(USER.message_rece,row[0]);
                                if(j%3==0)
                                    strcat(USER.message_rece,"\n\n");
                                mysql_free_result(res);
                            }
                        }
                        strcat(USER.message_rece,"\n\n");
                        if(send(fd,&USER,sizeof(USER),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                    else
                    {
                        strcpy(USER.message_rece,"no");
                        if(send(fd,&USER,sizeof(USER),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                }
                break;
            }
        case 15://群聊
            {
                USER.back=13;
                memset(sql,0,sizeof(sql));
                memset(other_friends,0,sizeof(other_friends));
                sprintf(sql,"SELECT group_users FROM groups WHERE group_name='%s';",USER.gpname);
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                row=mysql_fetch_row(res);
                strcpy(other_friends,row[0]);
                mysql_free_result(res);
                for(i=0;i<strlen(other_friends);i+=3)
                {
                    memset(ptr,0,sizeof(ptr));
                    ptr[0]=other_friends[i];
                    ptr[1]=other_friends[i+1];
                    other_id=atoi(ptr);
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"SELECT sockfd,on_out,username FROM userlogin WHERE id=%d;",other_id);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    res=mysql_store_result(&mysql);
                    row=mysql_fetch_row(res);
                    other_fd=atoi(row[0]);
                    my_on_out=atoi(row[1]);
                    memset(my_name,0,sizeof(my_name));
                    strcpy(my_name,row[2]);
                    memset(send_time_mation,0,sizeof(send_time_mation));
                    strcpy(send_time_mation,USER.sendtime);
                    send_time_mation[strlen(send_time_mation)]='\0';
                    memset(send_mation,0,sizeof(send_mation));
                    strcpy(send_mation,USER.message);
                    send_mation[strlen(send_mation)]='\0';
                    if(k==0)
                    {
                        memset(sql,0,sizeof(sql));
                        sprintf(sql,"INSERT INTO chat_groups(sendg_name,gr_name,cg_time,cg_mation)VALUES('%s','%s','%s','%s');",USER.uname,USER.gpname,send_time_mation,send_mation);
                        if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                        {
                            printf("query:%s\n",mysql_error(&mysql));
                        }
                    }
                    k++;
                    if(my_on_out==1)//在线
                    {
                        strcpy(USER.sendtime,send_time_mation);
                        strcpy(USER.message_rece,send_mation);
                        if(send(other_fd,&USER,sizeof(USER),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                    else//不在线
                    {
                        memset(sql,0,sizeof(sql));
                        sprintf(sql,"INSERT INTO formationbox(s_n,g_n,c_time,c_mation,fg2)VALUES('%s','%s','%s','%s',4);",USER.uname,USER.gpname,send_time_mation,send_mation);
                        if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                        {
                            printf("query:%s\n",mysql_error(&mysql));
                        }
                    }
                }
                break;
            }
        case 16://添加群
            {
                flag1=0;
                USER.back=14;
                memset(sql,0,sizeof(sql));
                memset(my_name,0,sizeof(my_name));
                memset(my_other,0,sizeof(my_other));
                memset(other_friends,0,sizeof(other_friends));
                memset(other_name,0,sizeof(other_name));
                strcpy(sql,"SELECT group_name,group_owner,group_users FROM groups;");
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                while((row=mysql_fetch_row(res)))
                {
                    if(strcmp(row[0],USER.gpname)==0)
                    {
                        flag1=1;
                        strcpy(my_name,row[1]);
                        strcpy(other_friends,row[2]);
                        break;
                    }
                }
                mysql_free_result(res);
                if(flag1==0)
                {
                    strcpy(USER.message_rece,"ns");
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                else
                {
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"SELECT id FROM userlogin WHERE username='%s';",USER.uname);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    res=mysql_store_result(&mysql);
                    row=mysql_fetch_row(res);
                    my_id=atoi(row[0]);
                    mysql_free_result(res);
                    for(i=0;i<strlen(other_friends);i+=3)
                    {
                        flag1=2;
                        my_other[0]=other_friends[i];
                        my_other[1]=other_friends[i+1];
                        if(atoi(my_other)==my_id)
                        {
                            flag1=3;
                            break;
                        }
                    }
                    if(flag1==3)
                    {
                        strcpy(USER.message_rece,"no");
                        if(send(fd,&USER,sizeof(USER),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                    else
                    {
                        memset(sql,0,sizeof(sql));
                        strcpy(other_name,USER.message);
                        sprintf(sql,"INSERT into formationbox(s_n,r_n,g_n,c_mation,fg2)VALUES('%s','%s','%s','%s',2);",USER.uname,my_name,USER.gpname,other_name);
                        if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                        {
                            printf("query:%s\n",mysql_error(&mysql));
                        }
                        strcpy(USER.message_rece,"yes");
                        if(send(fd,&USER,sizeof(USER),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                }
                break;
            }
        case 17://离线消息//添加的离线消息
            {
                flag1=0;
                USER.back=15;
                memset(sql,0,sizeof(sql));
                strcpy(sql,"SELECT r_n,s_n,ber,c_mation,g_n,fg2 FROM formationbox;");
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                while((row=mysql_fetch_row(res)))
                {
                    if(strcmp(row[0],USER.uname)==0)
                    {
                        flag1=1;
                        switch(atoi(row[5]))
                           {
                            case 1://添加好友
                            {
                                strcat(USER.message_rece,"\t\t");
                                strcat(USER.message_rece,row[2]);
                                strcat(USER.message_rece," :");
                                strcat(USER.message_rece,row[1]);
                                strcat(USER.message_rece," :");
                                strcat(USER.message_rece,row[3]);
                                strcat(USER.message_rece," \n");
                                break;
                            }
                            case 2://添加群
                            {
                                strcat(USER.message_rece,"\t\t");
                                strcat(USER.message_rece,row[2]);
                                strcat(USER.message_rece,".: ");
                                strcat(USER.message_rece,row[4]);
                                strcat(USER.message_rece,":\n");
                                strcat(USER.message_rece,row[1]);
                                strcat(USER.message_rece," :");
                                strcat(USER.message_rece,row[3]);
                                strcat(USER.message_rece," \n");
                                break;
                            }
                            case 5://文件收发
                               {
                                   strcat(USER.message_rece,"\t\t");
                                   strcat(USER.message_rece,row[2]);
                                   strcat(USER.message_rece,".:");
                                   strcat(USER.message_rece,row[1]);
                                   strcat(USER.message_rece," :");
                                   strcat(USER.message_rece,row[3]);
                                   strcat(USER.message_rece," \n");
                               }
                        }
                    }
                }
                mysql_free_result(res);
                if(flag1==0)
                {
                    strcpy(USER.message_rece,"no");//没有消息
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                else if(flag1==1)
                {
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                break;
            }
        case 18://离线消息//群，好友发的消息
            {
                flag1=0;
                USER.back=16;
                memset(sql,0,sizeof(sql));
                strcpy(sql,"SELECT r_n,s_n,g_n,c_time,c_mation,fg2 FROM formationbox;"); 
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                while((row=mysql_fetch_row(res)))
                {
                    if((strcmp(row[0],USER.uname)==0)&&((atoi(row[5])==3)||(atoi(row[5])==4)))
                    {
                        flag1=1;
                        switch(atoi(row[5]))
                           {
                            case 3://好友
                            {
                                strcat(USER.message_rece,"\t\t");
                                strcat(USER.message_rece,row[1]);
                                strcat(USER.message_rece," :");
                                strcat(USER.message_rece,row[3]);
                                strcat(USER.message_rece,":\n");
                                strcat(USER.message_rece,"\t\t");
                                strcat(USER.message_rece,row[4]);
                                strcat(USER.message_rece," \n");
                                break;
                            }
                            case 4://群
                            {
                                strcat(USER.message_rece,"\t\t");
                                strcat(USER.message_rece,row[2]);
                                strcat(USER.message_rece,"--");
                                strcat(USER.message_rece,row[1]);
                                strcat(USER.message_rece,": ");
                                strcat(USER.message_rece,row[3]);
                                strcat(USER.message_rece," \n");
                                strcat(USER.message_rece,row[4]);
                                strcat(USER.message_rece," \n");
                                break;
                            }
                        }
                    }
                }
                mysql_free_result(res);
                if(flag1==0)
                {
                    strcpy(USER.message_rece,"no");//没有消息
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                else if(flag1==1)
                {
                    if(send(fd,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                }
                break;
            }
        case 19://接受请求的信息
            {
                memset(other_groups,0,sizeof(other_groups));
                memset(my_friends,0,sizeof(my_friends));
                memset(other_friends,0,sizeof(other_friends));
                memset(my_other,0,sizeof(my_other));
                memset(other_my,0,sizeof(other_my));
                memset(sql,0,sizeof(sql));
                sprintf(sql,"SELECT fg2 FROM formationbox WHERE ber='%d';",USER.back);
                if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                {
                    printf("query:%s\n",mysql_error(&mysql));
                }
                res=mysql_store_result(&mysql);
                row=mysql_fetch_row(res);
                other_fd=atoi(row[0]);
                mysql_free_result(res);
                if(other_fd==1)//好友申请
                {
                    sprintf(sql,"SELECT s_n,r_n FROM formationbox WHERE ber='%d';",USER.back);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    res=mysql_store_result(&mysql);
                    row=mysql_fetch_row(res);
                    strcpy(my_other,row[0]);
                    strcpy(other_my,row[1]);
                    mysql_free_result(res);
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"SELECT id,friends FROM userlogin WHERE username='%s';",my_other);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    res=mysql_store_result(&mysql);
                    row=mysql_fetch_row(res);
                    my_id=atoi(row[0]);
                    strcpy(M_id,row[0]);
                    strcpy(my_friends,row[1]);
                    mysql_free_result(res);
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"SELECT id,friends FROM userlogin WHERE username='%s';",other_my);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    res=mysql_store_result(&mysql);
                    row=mysql_fetch_row(res);
                    other_id=atoi(row[0]);
                    strcpy(O_id,row[0]); 
                    strcpy(other_friends,row[1]);
                    mysql_free_result(res);
                    if((other_id>=0)&&(other_id<10))
                    {
                        if((other_id)==1)
                            strcat(my_friends,"|01\0");
                        else if((other_id)==2)
                            strcat(my_friends,"|02\0");
                        else if((other_id)==3)
                            strcat(my_friends,"|03\0");
                        else if((other_id)==4)
                            strcat(my_friends,"|04\0");
                        else if((other_id)==5)
                            strcat(my_friends,"|05\0");
                        else if((other_id)==6)
                            strcat(my_friends,"|06\0");
                        else if((other_id)==7)
                            strcat(my_friends,"|07\0");
                        else if((other_id)==8)
                            strcat(my_friends,"|08\0");
                        else if((other_id)==9)
                            strcat(my_friends,"|09\0");
                    }
                    else
                    {
                        my_friends[strlen(my_friends)]='|';
                        strcat(my_friends,O_id);
                    }
                    if((my_id>=0)&&(my_id<10))
                    {
                        if((my_id)==1)
                            strcat(other_friends,"|01\0");
                        else if((my_id)==2)
                            strcat(other_friends,"|02\0");
                        else if((my_id)==3)
                            strcat(other_friends,"|03\0");
                        else if((my_id)==4)
                            strcat(other_friends,"|04\0");
                        else if((my_id)==5)
                            strcat(other_friends,"|05\0");
                        else if((my_id)==6)
                            strcat(other_friends,"|06\0");
                        else if((my_id)==7)
                            strcat(other_friends,"|07\0");
                        else if((my_id)==8)
                            strcat(other_friends,"|08\0");
                        else if((my_id)==9)
                            strcat(other_friends,"|09\0");
                    }
                    else
                    {
                        other_friends[strlen(other_friends)]='|';
                        strcat(other_friends,M_id);
                    }
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"UPDATE userlogin set friends='%s'WHERE id=%d;",my_friends,my_id);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"UPDATE userlogin set friends='%s'WHERE id=%d;",other_friends,other_id);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"DELETE FROM formationbox WHERE ber='%d';",USER.back);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                }
                else if(other_fd==2)//加群申请
                {
                    memset(my_other,0,sizeof(my_other));
                    memset(my_groups,0,sizeof(my_groups));
                    memset(other_friends,0,sizeof(other_friends));
                    memset(other_groups,0,sizeof(other_groups));
                    memset(g_users,0,sizeof(g_users));
                    sprintf(sql,"SELECT s_n,g_n FROM formationbox WHERE ber='%d';",USER.back);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    res=mysql_store_result(&mysql);
                    row=mysql_fetch_row(res);
                    strcpy(my_other,row[0]);
                    strcpy(other_groups,row[1]);
                    mysql_free_result(res);
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"SELECT id,groups FROM userlogin WHERE username='%s';",my_other);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    res=mysql_store_result(&mysql);
                    row=mysql_fetch_row(res);
                    my_id=atoi(row[0]);
                    strcpy(M_id,row[0]);
                    strcpy(my_groups,row[1]);
                    mysql_free_result(res);
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"SELECT group_id,group_users FROM groups WHERE group_owner='%s';",USER.uname);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    res=mysql_store_result(&mysql);
                    row=mysql_fetch_row(res);
                    other_id=atoi(row[0]);
                    strcpy(O_id,row[0]);
                    strcpy(other_friends,row[1]);
                    mysql_free_result(res);
                    if((other_id>=0)&&(other_id<10))
                    {
                        if((other_id)==1)
                            strcat(my_groups,"|01\0");
                        else if((other_id)==2)
                            strcat(my_groups,"|02\0");
                        else if((other_id)==3)
                            strcat(my_groups,"|03\0");
                        else if((other_id)==4)
                            strcat(my_groups,"|04\0");
                        else if((other_id)==5)
                            strcat(my_groups,"|05\0");
                        else if((other_id)==6)
                            strcat(my_groups,"|06\0");
                        else if((other_id)==7)
                            strcat(my_groups,"|07\0");
                        else if((other_id)==8)
                            strcat(my_groups,"|08\0");
                        else if((other_id)==9)
                            strcat(my_groups,"|09\0");
                    }
                    else
                    {
                        my_groups[strlen(my_groups)]='|';
                        strcat(my_groups,O_id);
                    }
                    if((my_id>=0)&&(my_id<10))
                    {
                        if((my_id)==1)
                            strcat(other_friends,"|01\0");
                        else if((my_id)==2)
                            strcat(other_friends,"|02\0");
                        else if((my_id)==3)
                            strcat(other_friends,"|03\0");
                        else if((my_id)==4)
                            strcat(other_friends,"|04\0");
                        else if((my_id)==5)
                            strcat(other_friends,"|05\0");
                        else if((my_id)==6)
                            strcat(other_friends,"|06\0");
                        else if((my_id)==7)
                            strcat(other_friends,"|07\0");
                        else if((my_id)==8)
                            strcat(other_friends,"|08\0");
                        else if((my_id)==9)
                            strcat(other_friends,"|09\0");
                    }
                    else
                    {
                        other_friends[strlen(other_friends)]='|';
                        strcat(other_friends,M_id);
                    }
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"UPDATE userlogin SET groups='%s'WHERE id=%d;",my_groups,my_id);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                        }
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"UPDATE groups SET group_users='%s'WHERE group_id=%d;",other_friends,other_id);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"DELETE FROM formationbox WHERE ber='%d';",USER.back);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                }
                else
                {
                    USER.back=17;
                    memset(my_other,0,sizeof(my_other));
                    memset(other_my,0,sizeof(other_my));
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"SELECT r_n,c_mation FROM formationbox WHERE ber='%d';",USER.back);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    res=mysql_store_result(&mysql);
                    row=mysql_fetch_row(res);
                    strcpy(my_other,row[0]);
                    strcpy(other_my,row[1]);
                    mysql_free_result(res);
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"SELECT sockfd FROM userlogin WHERE username='%s';",my_other);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    res=mysql_store_result(&mysql);
                    row=mysql_fetch_row(res);
                    other_id=atoi(row[0]);
                    mysql_free_result(res);
                    int fd;
                    int len,len2;
                    if((fd=open(other_my,O_RDONLY))==-1)
                    {
                        my_err("open",__LINE__);
                    }
                    if(lseek(fd,0,SEEK_END)==-1)
                    {
                        my_err("lseek",__LINE__);
                    }
                    if((len=lseek(fd,0,SEEK_CUR))==-1)
                    {
                        my_err("lseek",__LINE__);
                    }
                    if(lseek(fd,0,SEEK_SET)==-1)
                    {
                        my_err("lseek",__LINE__);
                    }
                    USER.size=len;
                    while(1)
                    {
                        len2+=read(fd,USER.message,sizeof(USER.message));
                        if(len2==len)
                        {
                            USER.flag2=1;
                            if(send(USER.fd,&USER,sizeof(USER),0)==-1)
                            {
                                my_err("send",__LINE__);
                            }
                            break;
                        }
                        if(send(other_id,&USER,sizeof(USER),0)==-1)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                }
                break;
            }
            case 20:
                {
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"UPDATE userlogin SET on_out=0,sockfd=0 WHERE sockfd='%d';",fd);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    break;
                }
            case 21:
                {
                    int fd;
                    USER.back=18;
                    if((fd=open(USER.filename,O_CREAT|O_RDWR|O_APPEND,0644))==-1)
                    {
                        my_err("open",__LINE__);
                    }
                    while(1)
                    {
                        if(write(fd,USER.message,sizeof(USER.message))==-1)
                        {
                            my_err("write",__LINE__);
                        }
                        close(fd);
                        if(USER.flag2==1)
                        {
                            break;
                        }
                    }
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"INSERT INTO formationbox(s_n,r_n,c_mation,fg2)VALUES('%s','%s','%s',5);",USER.uname,USER.othername,USER.filename);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    strcpy(USER.message_rece,"[tips]:you have a message\n");
                    memset(sql,0,sizeof(sql));
                    sprintf(sql,"SELECT sockfd FROM userlogin WHERE username='%s';",USER.othername);
                    if(0!=mysql_real_query(&mysql,sql,strlen(sql)))
                    {
                        printf("query:%s\n",mysql_error(&mysql));
                    }
                    res=mysql_store_result(&mysql);
                    row=mysql_fetch_row(res);
                    my_id=atoi(row[0]);
                    if(send(my_id,&USER,sizeof(USER),0)==-1)
                    {
                        my_err("send",__LINE__);
                    }
                    break;
                }
    }
    mysql_close(&mysql);
}
