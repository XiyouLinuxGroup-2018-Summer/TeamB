/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月17日 星期五 08时31分40秒
 ************************************************************************/
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<time.h>
#include<unistd.h>
#include<sys/types.h>
#include<arpa/inet.h>


#define MAXSIZE 1024
#define SERV_PORT 8787//端口号
#define FDSIZE 1024
#define EPOLLEVENTS 20

//flag的选择：
//1.登录  2.注册  3.添加好友  4.删除好友  5.显示好友列表  6.查询好友  7.私聊  8.查聊天记录  9.创建群
//10.解散群  11退出群  12.查找群  13.显示群列表  14.显示群成员  15.群聊  16.离线消息

void handle_connection();
int menu_choice();
int menu_log();
int uformation();


//错误处理函数
void my_err(const char *err_string,int line)
{
    fprintf(stderr,"line:%d ",line);
    perror(err_string);
    exit(1);
}

//记得要初始化！！！
struct use 
{
    int flag;//你的选择
    int back;//返回值
    char uname;//名字
    char upassword;//密码
    char message[MAXSIZE];//发送的东西
}USER;

int main(int argc,char *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;
    //建立套接字并向服务器请求连接
    sockfd=socket(AF_INET,SOCK_STREAM,0);//建立套接字
    bzero(&servaddr,sizeof(servaddr));//初始化0
    //设置sockaddr_in
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(SERV_PORT);
    inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
    //连接服务器
    if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)))
    {
        perror("connect:");
        exit(1);
    }
    handle_connection(sockfd);
    close(sockfd);
    return 0;
}

void handle_connection(int sockfd)
{
    int choice1;//登录注册页面的选项
    int choice2;//用户登录是否正确的选项
    int choice3;//用户登录后的主页面
    int epollfd;//定义一个多路复用的句柄
    struct epoll_event events[EPOLLEVENTS];
    int ret;//有多少个等待的线程
    do
    {
        choice1=menu_log();
        printf("%d\n",choice1);
        switch(choice1)
        {
            case 1://登录
                {
                    printf("ha\n");
                    choice2=uformation(sockfd);
                    if(choice2==1)//登陆成功
                    {
                        do
                        {
                            choice3=menu_choice();
                            switch(choice3)
                            {
                                case 1://添加好友 要征得对方同意
                                    {
                                        break;
                                    }
                                case 2://删除好友
                                    {
                                        break;
                                    }
                                case 3://显示好友列表
                                    {
                                        break;
                                    }
                                case 4://查询好友
                                    {
                                        break;
                                    }
                                case 5://私聊
                                    {
                                        break;
                                    }
                                case 6://查聊天记录
                                    {
                                        break;
                                    }
                                case 7://创建群
                                    {
                                        break;
                                    }
                                case 8://解散群
                                    {
                                        break;
                                    }
                                case 9://退出群
                                    {
                                        break;
                                    }
                                case 10://查找群
                                    {
                                        break;
                                    }
                                case 11://显示群列表
                                    {
                                        break;
                                    }
                                case 12://显示群成员
                                    {
                                        break;
                                    }
                                case 13://群聊
                                    {
                                        break;
                                    }
                                case 14://离线消息
                                    {
                                        break;
                                    }
                                case 0://返回上一层
                                    {
                                        break;
                                    }
                            }
                        }while(choice3!=0);
                    }
                    else
                    {
                        printf("\t\t登录失败\n");
                    }
                    break;
                }
            case 2://注册
                {
                    break;
                }
            case 0:break;
        }
    }while(choice1!=0);
}


//menu_choice页面：
int menu_choice(void)
{
    int choice;
    printf("\t\t**************************************\n");
    printf("\t\t|             欢迎来到主页面         |\n");
    printf("\t\t|             1.添加好友             |\n");
    printf("\t\t|             2.删除好友             |\n");
    printf("\t\t|             3.显示好友列表         |\n");
    printf("\t\t|             4.查询好友             |\n");
    printf("\t\t|             5.私聊                 |\n");
    printf("\t\t|             6.查聊天记录           |\n");
    printf("\t\t|             7.创建群               |\n");
    printf("\t\t|             8.解散群               |\n");
    printf("\t\t|             9.退出群               |\n");
    printf("\t\t|             10.查找群              |\n");
    printf("\t\t|             11.显示群列表          |\n");
    printf("\t\t|             12.显示群成员          |\n");
    printf("\t\t|             13.群聊                |\n");
    printf("\t\t|             14.离线消息            |\n");
    printf("\t\t|             0.退出主页面           |\n");
    printf("\t\t**************************************\n");
    printf("\t\t*请选择你的选项：");
    scanf("%d",&choice);
    return choice;
}


//menu_log页面：
int menu_log(void)
{
    int choice;
    printf("\t\t**************************************\n");
    printf("\t\t|             欢迎来到chat           |\n");
    printf("\t\t|             1.登录                 |\n");
    printf("\t\t|             2.注册                 |\n");
    printf("\t\t**************************************\n");
    printf("\t\t请输入您的选择:");
    scanf("%d",&choice);
    printf("choice==%d\n",choice);
    return choice;
}


int uformation(int sockfd)
{
    printf("uformation\n");
    int i;
    printf("\t\t* 请输入用户名：\n");
    gets(USER.uname);
    printf("\t\t* 请输入密码：\n");
    gets(USER.upassword);
    USER.flag=1;
    if(send(sockfd,&USER,sizeof(struct use),0)==-1)
    {
        my_err("send",__LINE__);
    }
    if(recv(sockfd,&i,sizeof(i),0)<0)
    {
        my_err("recv",__LINE__);
    }
    if(i==1)//成功
        return 1;
    else//失败
        return 0;
}


