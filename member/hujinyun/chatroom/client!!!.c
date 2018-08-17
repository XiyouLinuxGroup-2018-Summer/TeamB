//开始客户端
#include<stdio.h>

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

#define MAX_EVENTS 64
#define MAX_CHAR 300
#define UAER_MAX 100
#define GROUP_MAX 50
#define EPOLL_MAX 200000

//定义类型
#define LOGIN 1
#define REGISTER 2
#define FRIEND_SEE 3
#define FRIEND_ADD 4
#define FRIEND_DEL 
#define POST 8000
struct usr{
    char usename[20];
    char password[20];
};
typedef struct friend_info{
    int statu;
    int mes_num;
    char name[MAX_CHAR];
}FRIEND_INFO;

typedef struct user_info{
    char username[MAX_CHAR];
    FRIEND_INFO friends[MAX_CHAR];
    int friend_num;
    char group[MAX_CHAR];
    int group_num;
}USER_INFO;


typedef struct data{  //收发信息
    char send_name[MAX_CHAR];
    char recv_name[MAX_CHAR];
    int send_fd;
    int recv_fd;
    char mes[MAX_CHAR];
}DATA;

typedef struct package{
    int type;
    DATA data;
}PACK;//包处理

//-------------------------------------------------------------
int sockfd;
char *IP = "128.0.0.233";
USER_INFO m_my_infor;


//--------------------------------------------------------------
void my_err(const char *err_string, int line);
int login_menu();
int get_choice(char *choice_t);
int login();
int send_login(char *username_t, char *password_t);
void send_pack(int type, char *send_name, char *recv_name,char *mes);
void registe();
int send_registe(char* username_t, char*password_t);
void init();
void init_client_pthread();


//错误处理函数
void my_err(const char *err_string, int line)
{
    fprintf(srderr,"line:%d", line);
    perror(err_string);
    exit(1);
}
//显示登录注册菜单选择
int login_menu()
{
    char choice_t[100];
    int choice;
    do
    {
        printf("\t\t\t\t********************\n");
        printf("\t\t\t\t******1.登录*********\n");
        printf("\t\t\t\t******2.注册*********\n");
        printf("\t\t\t\t********************\n");
        printf("%s",choice_t);
        choice = get_choice(choice_t);
        switch(choice)
        {
            case 1:
                    if(login() == 1)
                        return 1;
                    break;
            case 2:
                    registe();
                    break;
            default:
                    break;
        }
    }while(choice!=0);
    return 0;
}

//字符串解析成数字,防止乱码
int get_choice(char *choice_t)
{
    int choice = 0;
    for(int i=0; i<strlen(choice_t); i++)
    {
        if(choice_t[i]<'0' || choice_t[i]>'9')
            return -1;  
    }
    choice = atoi(choice_t);
    return choice;
}
//向客户端发送登录请求,根据返回内容提示用户是否成功登陆
int login()
{
    int flag = 0;
    int login_flag = 0;
    char username_t [MAX_CHAR];
    char password_t [MAX_CHAR];
    printf("\t\t\t\tusername:");
    scanf("%s",username_t);
    printf("\t\t\t\tpassword:");
    scanf("%s",password_t);
    login_flag = send_login(username_t,password_t);
    if(login_flag == 2)
    {
        printf("该用户不存在\n");
        return 0;
    }
    if(login_flag == 3)
    {
        printf("该用户正在登录呢\n");
        return 0;
    }
    if(login_flag == 0)
    {
        printf("密码错误!\n");
        return 0;
    }
    strcpy(m_my_infor.username,username_t);
    printf("登陆成功!\n");
    return 1;
}
//向服务端发送登录信息
int send_login(char *username_t, char *password_t)
{
    PACK recv_login_t;
    int  login_judge_flag = 0;
    send_pack(LOGIN, username_t, "server", password_t);  //LOGIN

    if(recv(sockfd, &recv_login_t, sizeof(PACK),0)<0)
        my_err("recv", __LINE__);
    login_judge_flag = recv_login_t.data.mes[0]-48;  //接收返回recv,返回数值0 2 3
    return login_judge_flag;
}

//根据参数信息发送包
void send_pack(int type, char *send_name, char *recv_name,char *mes)
{
    PACK pack_send_pack;
    time_t timep;
    pack_send_pack.type = type;
    strcpy(pack_send_pack.data.send_name,send_name);
    strcpy(pack_send_pack.data.recv_name,recv_name);
    strcpy(pack_send_pack.data.mes,mes);
    time(&timep);
    if(send(sockfd,&pack_send_pack,sizeof(PACK),0)<0)
        my_err("send",__LINE__);
}

//注册用户,根据输入信息向客户端发送注册请求,根据返回值,提示用户
void registe()
{
    int flag = 0;
    flag = REGISTER;
    char username_t[MAX_CHAR];
    char password_t[MAX_CHAR];

    printf("\t\t\t\tusername:");
    scanf("%s",username_t);
    printf("\t\t\t\tpassname:");
    scanf("%s",password_t);
    if(send_registe(username_t,password_t))//登陆成功
        printf("注册成功!");
    else
        printf("用户名已被注册啦,请换一个尝试吧\n");
}
//向服务端发送注册信息
int send_registe(char* username_t, char*password_t)
{
    PACK recv_registe_t;
    int registe_judge_flag = 0;
    send_pack(REGISTER, username_t, "server",password_t);

    if(recv(sockfd, &recv_registe_t,sizeof(PACK),0)<0)
        my_err("recv",__LINE__);
    registe_judge_flag = recv_registe_t.data.mes[0]-48;
    return registe_judge_flag;
}

//启动,连接服务器
void init()
{
    printf("客户端启动----------------\n");
    sockfd = socket(AF_INET,SOCK_STREAM,0); //创建TCP套接字
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;    
    addr.sin_port = htons(POST);
    addr.sin_addr.s_addr = inet_addr(IP);
    if(connect(sockfd,(struct sockaddr*)&addr,sizeof(addr))<0)
        {my_err("connect",__LINE__);
        printf("无法连接到服务器...客户端启动失败\n");
        exit(-1);
        }
    printf("客户端启动成功!\n");
}
/*
//开启客户端持续工作线程
void init_client_pthread()
{
    pthread_t pid_deal_statu,pid_recv;
    pthread_create(&pid_deal_statu, NULL, (void*)deal_statu, NULL);//不停更新好友状态
    pthread_create(&pid_recv, NULL, (void*)clien_recv_thread,NULL);//不停接受发送来的数据
}

//更新好友状态状态:
//一般这种做法不是客户端的事情。而是服务器端的事情。

//1、 服务器知道每个用户的登录状态
//2、 每个用户登录的时候，服务器根据你好友列表的状态即可主动发送在线用户状态给你
//3、 客户端只需要解析服务器发送过来的状态信息以及好友列表即可

//根据发来数据包及时更新好友的状态(此函数只用于处理服务端不断发送过来的数据包来更新好友状态,不包含更新请求等)
void *deal_statu(void *arg)
{
    int i;
    while(1)
    {
        pthread_mutex_lock(&mutex_local_user);//mutex_local_user
        for(i=1; i<m_recv_num_friend_see; i++)
        {
            change_statu(m_pack_recv_num_friend_see[i]);
        }
        m_recv_num_friend_see=0;
        pthread_mutex_unlock(&mutex_lock_user);
        usleep(1);
    }
}

//根据服务端发送来的包,利用字符串解析,更新当前好友
void change_statu(PACK pack_deal_statu_t)
{
    int count = 0;
    m_my_infor.friend_num = pack_deal_statu_t.data.mes[cont++];
    for()
}
//一致接受客户端发送来的包,并储存入数组
*/
int main(int argc, char **argv)
{
  //  signal(SIGINT, sig_close); //关闭Ctrl+c
    init();//启动并连接服务器
    if(login_menu() == 0)
        return 0;

//    init_client_pthread(); //开启线程
 //   main_menu(); //主菜单
    return 0;

}