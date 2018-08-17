/*要开始动手写了epoll其实用的还不熟,先开始吧 8.16.14:30*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAX_EVENTS      64
#define MAX_CHAR        300
#define UAER_MAX        100 //网络在线人数
#define GROUP_MAX       50
#define EPOLL_MAX       200000
#define LISTEMAX        1000
#define FILE_MAX        50
#define NUM_MAX_DIGIT   10

#define DOWLINE         0
#define ONLINE          1
#define BUSY            2

#define LOGIN           1
#define REGISTER        2
#define FRIEND_SEE      3
#define FRIEND_ADD      4
#define FRIEND_DEL      5
#define GROUP_SEE       6
#define GROUP_CREATE    7
#define GROUP_JOIN      8
#define GROUP_QUIT      9
#define GROUP_DEL      10

#define LOG
#define MYSQL_OPEN

struct usr{
    char usename[20];
    char password[20];
};
//储存用户结构体
typedef struct infor_user
{   
    struct usr usr[UAER_MAX]; //用户名密码
    int statu; //用户状态
    int socket_id; //用户套接字id
    char friends[UAER_MAX][MAX_CHAR]; //好友信息
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
}DATA;

typedef struct pack
{
    int type;
    DATA fata;
}PACK;

//线程传参结构体?/??///?????????
typedef struct pthread_parameter{
    int a;
    char str1[SIZE_PASS_NAME];
    char str2[SIZE_PASS_NAME];
}PTHREAD_PAR;
//-------------------------------------------
int PORT 8000;
//-------------------------------------------

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
    if(bind(std,(struct sockaddr*)&sa, sizeof(struct sockaddr))== -1)
        return -1;
    return sfd;
}

int main(int argc, char **argv)
{
    int n;
    PACK recv_t;
    PACK *recv_pack;
    int socketfd;
    int fd_num;
    int err;
    struct sockaddr_in fromaddr;
    socklen_t len = sizeof(fromaddr);
    struct eppol_event ev, events[LISTEMAX];

    read_infor();//从文件中读取用户信息 (未实现)
    (未实现)
    //将日志存入文件
    if(log_file_fd = open("server_log.txt",O_WRONlY | O_CREAT | O_ADDEND, S_IRSUR | S_IWUSR)<0)
    
        my_err("open",__LINE__);
        return 0;
    }
    dup2(log_file_fd,1);//输入文件

    //连接mysql
    connect_mysql_init(); (未实现)
    pthread_mutex_init(&mutex,NULL); //初始化一个锁

    printf("服务器开始启动>>>>>>>>>>>>>>\n");
    init_server_pthread();(未实现)
    epollfd = epoll_create();//创建句柄

    listenfd = socket(AF_INET,SOCK_STREAM,0); //启动服务端套接字
    if(listenfd == -1)
    {
        perror("listenfd");
        printf("服务器启动失败...创建服务端套接字失败...");
        exit(-1);
    }
}

//从文件中读取用户信息
int read_infor()
{

}
