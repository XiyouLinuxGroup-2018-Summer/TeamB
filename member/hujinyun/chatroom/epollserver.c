#include <mysql/mysql.h> 
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <math.h>
 

#define PORT            8000
#define MAX_EVENTS      64
#define MAX_CHAR        300
#define USER_MAX        100 //网络在线人数
#define GROUP_MAX       50
#define EPOLL_MAX       200000
#define LISTEMAX        1000
#define FILE_MAX        50
#define NUM_MAX_DIGIT   10

#define DOWNLINE        0
#define ONLINE          1
#define BUSY            2

#define LOGIN           1   //登录
#define REGISTE         2   //注册
#define FRIEND_SEE      3
#define FRIEND_ADD      4
#define FRIEND_DEL      5
#define GROUP_SEE       6
#define GROUP_CREATE    7
#define GROUP_JOIN      8
#define GROUP_QUIT      9
#define GROUP_DEL      10
#define EXIT           -1
#define LOG
#define MYSQL_OPEN


struct usr{
    char username[20];
    char password[20];
};
//储存用户结构体
typedef struct infor_user
{   
//    struct usr usr[UAER_MAX]; //用户名密码
    char username[MAX_CHAR];
    char password [MAX_CHAR];
    int statu; //用户状态
    int socket_id; //用户套接字id
    char friends[USER_MAX][MAX_CHAR]; //好友信息
    int friends_num;//好友个数
    char group[GROUP_MAX][MAX_CHAR]; //群信息
    int group_num;//群个数
}INFO_USER;

//包结构体
typedef struct data{
    char send_name[MAX_CHAR];
    char recv_name[MAX_CHAR];
    int send_fd;
    int recv_fd;
    char mes[MAX_CHAR*2];
}DATA;

typedef struct pack
{
    int type;
    DATA data;
}PACK;
/*
//线程传参结构体?/??///?????????
typedef struct pthread_parameter{
    int a;
    char str1[SIZE_PASS_NAME];
    char str2[SIZE_PASS_NAME];
}PTHREAD_PAR;*/
//-------------------------------------------
int listenfd,epollfd;
int log_file_fd;
int user_infor_fd;
int group_infor_fd;

pthread_mutex_t mutex;
pthread_mutex_t mutex_recv_file;
pthread_mutex_t mutex_check_file;

INFO_USER m_infor_user[USER_MAX]; //在线的用户列表
int m_user_num;  //在线的用户个数

PACK m_pack_send[LISTEMAX];
int m_send_num;

//---------数据库全局变量----------------------
MYSQL mysql;
MYSQL_RES *res = NULL;
MYSQL_ROW row;
char query_str[MAX_CHAR*4];
int rc,fields;
int rows;
//-------------------------------------------

int connect_mysql_init();
void init_server_pthread();
void *serv_send_thread(void *arg);
void * recv_pack(void *arg);
void *deal(void *recv_park_t);
int find_userinfor(char *username_t);
void login(PACK *recv_pack);
void registe(PACK *recv_pack);
void send_pack(PACK *pack_send_pack_t);
int judge_usename_same(char *usename_t);
void print_infor_user();
void read_infor();//从文件中读取用户信息
void mysql_close();
void signal_close(int i);
int read_infor();
int write_infor();

void my_err(const char *err_string, int line)
{
    fprintf(stderr,"line: %d",line);
    perror("err_string");
    exit(-1);
}



//1.创建与连接端口
int create_and_bind(int port)
{
    int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sfd == -1)
        return -1;

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PORT);
    sa.sin_addr.s_addr=htonl(INADDR_ANY);
    if(bind(sfd,(struct sockaddr*)&sa, sizeof(struct sockaddr))== -1)
        return -1;
    return sfd;
}

//初始化数据库
int connect_mysql_init()
{
    if(NULL == mysql_init(&mysql))
    {
        printf("mysql_init(): %s\n",mysql_error(&mysql));//初始化mysql句柄
        return -1;
    }
    if(NULL== (mysql_real_connect(&mysql,"localhost","root","199951k","mychat",0,NULL,0)))
       {
            printf("mysql_real_connect():%s\n",mysql_error(&mysql));
       } 
    printf("connect mysql successful!\n");
}

//初始化开启线程
void init_server_pthread()
{
    printf("\ninit_server_pthread\n");
    pthread_t pid_send, pid_file_check;
    pthread_create(&pid_send,NULL,(void*)serv_send_thread,NULL); //单开线程一直send
//    pthread_create(&pid_file,NULL,(void*)pthread_check_file,NULL);(未实现) //单开线程检测文件状态,当文件传输失败发提醒给客户端(..传送文件待定)
}

//发送包函数,不断遍历包存储的数组,如果要发送的客户端在线,则发送,否则一直等到其上线,实现离线功能
void *serv_send_thread(void *arg)
{
    int user_statu = DOWNLINE;
 //   int id_stop;
    int i,j,recv_fd_t,recv_fd_online;
    while(1)
    {
        pthread_mutex_lock(&mutex);//上锁,保护存包数组
        user_statu = DOWNLINE;
        for(i= m_send_num-1; i>=0; i--)
        {
            for(j=1;j<=m_user_num; j++)
            {
                if(strcmp(m_pack_send[i].data.recv_name,m_infor_user[j].username) == 0)
                {
                    user_statu = m_infor_user[j].statu;
                    if(user_statu == ONLINE)
                        recv_fd_online = m_infor_user[j].socket_id;
                    break;
                }
            }
            //上线,则发包
            if(user_statu == ONLINE || m_pack_send[i].type == LOGIN || m_pack_send[i].type == REGISTE)
            {
                if(user_statu == ONLINE)
                    recv_fd_t = recv_fd_online;
                else
                    recv_fd_t = m_pack_send[i].data.recv_fd; //查看包要发送的人是谁,发送出去

                if(send(recv_fd_t, &m_pack_send[i],sizeof(PACK),0)<0)
                {
                    my_err("send",__LINE__);
                }
                printf("send pack--------------\n");
                printf("type:   %d\n",m_pack_send[i].type);
                printf("from:   %s\n",m_pack_send[i].data.send_name);
                printf("to:     %s\n",m_pack_send[i].data.recv_name);
                printf("mes:    %s\n",m_pack_send[i].data.mes);
                printf("recv_fd:%s\n",m_pack_send[i].data.recv_fd);
                m_send_num--;
                printf("pack num now is:%d\n\n",m_send_num);

                for(j=i; j<=m_send_num&&m_send_num; j++)
                {
                    m_pack_send[j]=m_pack_send[j+1];
                }
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
        usleep(1);
    }
}

//处理包的函数,每接受一个包,开新线程,根据类型就能进行处理
void *deal(void *recv_park_t)
{
    int i;
    PACK *recv_pack = (PACK*)recv_park_t;
    printf("\n\n\ndeal function = %d\n",recv_pack->type);
    switch(recv_pack->type)
    {
        case LOGIN:
                    login(recv_pack);
                    break;
        case REGISTE:
                    registe(recv_pack);
                    break;
/*        case FRIEND_SEE:
                    send_statu(recv_pack);
                    break;
        case FRIEND_ADD:
                    friend_add(recv_pack);
                    break;
        case FRIEND_DEL:
                    friend_del(recv_pack);
                    break;
        case GROUP_CREATE:
                    group_create(recv_pack);
                    break;
        case GROUP_JOIN:
                    group_join(recv_pack);
                    break;
        case GROUP_DEL:
                    group_del(recv_pack);
                    break;
        case GROUP_QUIT:
                    group_quit(recv_pack);
                    break;
        case CHAT_ONE:
                    send_mes_to_one(recv_pack);
                    break;
        case CHAT__OMANY:
                    send_mes_to_group(recv_pack);
                    break;
        case MES_RECORD:
                    send_record(recv_pack);
                    break;
        case EXIT:
                    break;
*/
    }
}
//根据姓名找到用户信息
int find_userinfor(char *username_t)
{
    int i;
    if(m_user_num == 0) 
        return 0;
    for(i=1; i<=m_user_num; i++)
    {
        if(strcmp(m_infor_user[i].username, username_t) == 0)
        {
            return i;
        }
    }
    if(i == m_user_num+1)
        return 0;
}

//根据收到包的内容,找到登陆者的信息和密码作对比,并返回响应值
 void login(PACK *recv_pack)
 {
     int id = 0;
     char login_flag[10];

     if((id = find_userinfor(recv_pack->data.send_name)) == 0)//用户名不存在
    {
        login_flag[0] = '2';
    }
    else if(m_infor_user[id].statu == ONLINE)//已被登录了
    {
        login_flag[0] = '3';
    }
    else if(strcmp(m_infor_user[id].password,recv_pack->data.mes) == 0 )  //密码对
    {
        login_flag[0] = '1';
    }
    else //其余是密码错误情况
        login_flag[0] = '0';
        login_flag[1] = '\0';
    
    m_infor_user->socket_id  = recv_pack->data.send_fd;

    //服务端提示信息
    printf("\t\t\t*********login*********");
    printf("\t\t\t%s get online!\n",m_infor_user->username);
    printf("\t\t\tstatu:  %d",m_infor_user->statu);
    printf("\t\t\tsocket_id: %d",m_infor_user->socket_id);

    //对新要发送的包进行变量赋值
    strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
    strcpy(recv_pack->data.send_name,"server");
    recv_pack->data.recv_fd = recv_pack->data.send_fd;
    recv_pack->data.send_fd = listenfd;
    strcpy(recv_pack->data.mes,login_flag);
    
    send_pack(recv_pack);//发送包

    //使用土方法等待..,数据传输
    usleep(100);
    if(login_flag[0] == '1')
    m_infor_user->statu = LOGIN;
    free(recv_pack);
 }

void signal_close(int i)
{
    close(log_file_fd);
    write_infor();
    mysql_close();
    printf("服务器已关闭..");
    exit(1);

}
void mysql_close()
{
    mysql_free_result(res);
    mysql_close(&mysql);
}
//根据收到包的内容判断是否已注册,返回是否
void registe(PACK *recv_pack)
{
    char registe_flag[10];
    if(judge_usename_same(recv_pack->data.send_name))
    {
        registe_flag[0] = '1';
        
        //将信息添加进数组
        m_user_num++;
        strcpy(m_infor_user[m_user_num].username,recv_pack->data.send_name);
        strcpy(m_infor_user[m_user_num].password,recv_pack->data.mes);


        //服务端提示信息
        printf("registe success!\n");
        printf("username:%s\n", m_infor_user[m_user_num].username);
        printf("password:%s\n", m_infor_user[m_user_num].password);
        printf("user_num:%d\n\n", m_user_num);

        m_infor_user[m_user_num].statu = DOWNLINE;
    }
    else
        registe_flag[0] = '0';
    registe_flag[1] = '\0';

    strcpy(recv_pack->data.recv_name,recv_pack->data.send_name);
    strcpy(recv_pack->data.send_name,"server");
    strcpy(recv_pack->data.mes,registe_flag);
    recv_pack->data.recv_fd = recv_pack->data.send_fd;
    recv_pack->data.send_fd = listenfd;

    send_pack(recv_pack);
    usleep(100);  
    free(recv_pack);
}

 //发送包,因为有专门发送包的线程,所以只需将包的内容赋值到发送包的数组
 void send_pack(PACK *pack_send_pack_t)
 {
    pthread_mutex_lock(&mutex);
    memcpy(&(m_pack_send[m_send_num++]),pack_send_pack_t,sizeof(PACK));
    pthread_mutex_unlock(&mutex);     
 }

 //判断用户信息中是否有当前名字的信息,有返回0,没有返回1
 int judge_usename_same(char *usename_t)
 {
    int i;
    if(m_user_num == 0) return 1;
    for( i=1; i<=m_user_num; i++)
    {
        if(strcmp(m_infor_user->username,usename_t) == 0)
            return 0;
    }    
    if(i = m_user_num+1)
        return 1;
 }

int main(int argc, char **argv)
{
    int n;
    int i;
    pthread_t pid;
    PACK recv_t;
    PACK *recv_pack;
    int socketfd;
    int fd_num;
    int err;
    struct sockaddr_in fromaddr;
    socklen_t len = sizeof(fromaddr);
  //  struct eppoll_event ev, events[LISTEMAX];
    struct epoll_event ev;
    struct epoll_event events[LISTEMAX];
    

    read_infor();//从文件中读取用户信息 (未实现)???从数据库中读取文件之数组中
  
    //将日志存入文件
    if(log_file_fd = open("server_log.txt", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR)<0)//还没定义log_file_fd
    {
        my_err("open",__LINE__);
        return 0;
    }
    dup2(log_file_fd,1);//输入文件

    signal(SIGINT,signal_close);//退出CTRL+C
    
    //连接mysql
    connect_mysql_init(); 
    pthread_mutex_init(&mutex,NULL); //初始化一个锁

    printf("服务器开始启动>>>>>>>>>>>>>>\n");
    init_server_pthread(EPOLL_MAX);//开启线程
    epollfd = epoll_create(EPOLL_MAX);//创建句柄

    listenfd = socket(AF_INET,SOCK_STREAM,0); //启动服务端套接字
    if(listenfd == -1)
    {
        perror("listenfd");
        printf("服务器启动失败...创建服务端套接字失败...\n");
        exit(-1);
    }
    //设置超时机制,使其在一定时间后返回而不管是否有数据到来
    err=1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&err,sizeof(int)); 
    ev.data.fd=listenfd; //设置与要处理事件相关的文件描述符
    ev.events = EPOLLIN; //设置要处理的事件类型 LT模式,只要缓冲区有东西就一直汇报
    int s;
    s = epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);//注册epoll事件

    //准备网络通信地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listenfd,(struct sockaddr*)&addr,sizeof(addr))== -1)
    {
        perror("bind");
        printf("服务器启动失败...绑定失败\n");
        exit(-1);
    }
    printf("绑定成功");

    //设置监听
    if(listen(listenfd,10)== -1)
    {
        perror("listen");
        printf("服务器启动失败...设置监听失败\n");
        exit(-1);
    }
    printf("设置监听成功..\n");
    printf("初始化服务器成功..\n");
    printf("服务器开始服务..\n");

    while(1)
    {
        fd_num = epoll_wait(epollfd,events, EPOLL_MAX, -1);
        for(i=0; i<fd_num; i++)//好处:即用即完,即释放
        {
            //处理新接进来的套接字
            if(events[i].data.fd == listenfd)
            {
                socketfd = accept(listenfd,(struct sockaddr_in*)&fromaddr,&len);//fromaddr和len未定义
                printf("%d connect------------\n",socketfd);
                ev.data.fd = socketfd;
                ev.events = EPOLLIN; //设置监听事件可写
                //向句柄中新增套接字
                epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &ev);
            }
            else if(events[i].events & EPOLLIN)//接入的套接字有数据可读
            {
                n = recv(events[i].data.fd, &recv_t, sizeof(PACK),0);//读取数据
                recv_t.data.send_fd = events[i].data.fd;

                //输出收到包的消息    
                printf("pack-------------------------------\n");
                printf("type:   %d\n",recv_t.type);
                printf("from:   %s\n",recv_t.data.send_name);
                printf("to:     %s\n",recv_t.data.recv_name);
                printf("mes:    %s\n",recv_t.data.mes);
                printf("recv_fd:%s\n",recv_t.data.recv_fd);
                printf("send_pack_num:%d\n",m_send_num);
                
                if(n < 0)//recv错误
                {
                    close(events[i].data.fd); //错误处理必须有,<0说明没有从客户端收到东西,要关闭此文件描述符防止空间没释放
                    perror("recv");
                    continue;
                }
                else if(n == 0)//收到空包,代表下线
                {
                    //客户端下线后,把客户端状态设置为下线
                    for(int j=1; j<=m_user_num; j++)
                    {
                        if(events[i].data.fd == m_infor_user[j].socket_id);
                        {
                            printf("%s down line----------\n",m_infor_user[j].username);
                            m_infor_user[j].statu = DOWNLINE;
                            break;
                        }
                    }
                    ev.data.fd = events[i].data.fd;
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
                    close(events[i].data.fd);
                    print_infor_user();
                    continue;
                }
                int type = 0;
                type = recv_t.type;
                recv_pack = (PACK*)malloc(sizeof(PACK));
                memcpy(recv_pack, &recv_t,sizeof(PACK));

                //开启线程处理接收到的包,
                if(pthread_create(&pid,NULL,(void *)deal,(void*)recv_pack)!=0)//参数为recv_pack,注意将参数类型进行转换
                    my_err("pthread_create",__LINE__);
/*              if(type == FILE_SEND)
                {
                    printf("....");
                    usleep(1000);
                }*/
            }
        }

    }
    return 0;
}

//程序进行最开始,从文件中读取用户信息
int read_infor()
{
    INFO_USER read_file_t;
    if(user_infor_fd = open("user_infor.txt",O_RDONLY | O_CREAT,  S_IRUSR | S_IWUSR) <0 )
    {
        my_err("open",__LINE__);
        return -1;
    }
    int length;
    while(length = read(user_infor_fd, &read_file_t, sizeof(INFO_USER)) > 0)
    {
        read_file_t.statu = DOWNLINE;
        m_infor_user[++m_user_num] = read_file_t;
    }
    close(user_infor_fd);
    printf("have read the user_infor.txt!\n");
}
//服务端关闭时,吧信息写入文件中
int write_infor()
{
    INFO_USER read_file_t;
    if( (user_infor_fd = open("user_infor.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) <0)
    {
        my_err("open",__LINE__);
        return -1;
    }
    int length;
    for( int i = 1; i<=m_infor_user; i++)
    {
        if(write(user_infor_fd, m_infor_user+i, sizeof(INFO_USER)) <0 )
        {
            my_err("close",__LINE__);
            return -1;
        }
    }
    close(user_infor_fd);
    printf("have save the user_infor.txt\n");
    
//打印所有用户及其信息
void print_infor_user()
{
    for(int i=1; i<m_user_num; i++)
    {
        printf("user:       %d\n",i);
        printf("user_name:  %s\n",m_infor_user[i].username);
        printf("statu:      %d\n",m_infor_user[i].statu);
        printf("friend_num: %d\n",m_infor_user[i].friends_num);
        for(int j=1; j<=m_infor_user[i].friends_num; j++)
            printf("friend:     %s\n",m_infor_user[i].friends[j]);

        //预留打印群信息
        printf("user_num:   %d\n",m_infor_user[i].group_num);
        for(int j=1; j<=m_infor_user[i].group_num; j++)
            printf("group:      %s\n",m_infor_user[i].group[j]);
        printf("----------------------------------------\n\n\n");
    }
}

