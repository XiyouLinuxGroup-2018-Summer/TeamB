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
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<pthread.h>

#define MAXSIZE 1024
#define SERV_PORT 8686//端口号
#define FDSIZE 1024
#define EPOLLEVENTS 20
#define MAX 2048

struct use
{
    int flag;//你的选择
    int flag2;
    int back;//回来的选择
    int fd;//发过去的端口号
    int sd;//套接字
    int size;
    char uname[30];//客户端当前登录名
    char gpname[30];//组名
    char othername[30];//名字
    char upassword[30];//密码
    char message[MAXSIZE];//发送的东西
    char sendtime[50];
    char message_rece[MAXSIZE];//接收到的东西
    char filename[40];
    char path[50];
};

struct stu
{
    char stu_name[30];
    int stu_choice;
};

void my_err(const char *err_string,int line);//错误处理函数
void my_recv();//一直接受
void handle_connection();//处理连接
int menu_choice();//menu_choice页面
int menu_log();//menu_log页面
struct stu user_login();//登录函数
void user_logout();//注册函数
void del_friends();
void display_friends();
void find_friends();
void find_chat();
void create_group();
void drop_group();
void bye_group();
void find_group();
void show_group();
void show_group_user();
void add_friends();
void find_box();
void find_box_message();
void add_group();
void chat_friend();
void chat_groups();
void send_file();
void send_or_file();
void send_yes_file();



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
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(SERV_PORT);
    inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
    if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(struct sockaddr)))
    {
        perror("connect:");
        exit(1);
    }
    handle_connection(sockfd);
    close(sockfd);
    return 0;
}

void my_recv(void *ssd)
{
    int sockfd=*(int *)ssd;
    int num;
    int choice;
    char choice1;
    struct use USER;
    while(1)
    {
        num=recv(sockfd,&USER,MAX,0);
        if((MAX-num)==0)
        {
            switch(USER.back)
            {
                case 1://添加好友
                    {
                        if(strcmp(USER.message_rece,"no")==0)
                            printf("\t\t没有这个用户！\n");
                        else if(strcmp(USER.message_rece,"ns")==0)
                            printf("\t\t你和ta是好友哦～\n");
                        else if(strcmp(USER.message_rece,"yes")==0)
                            printf("\t\t已经发送请求，正在等待对方同意！\n");
                        break;
                    }
                case 2://删除好友
                    {
                        if(strcmp(USER.message_rece,"yes")==0)
                            printf("\t\t删除成功!\n");
                        else if(strcmp(USER.message_rece,"no")==0)
                            printf("\t\t删除失败，您没有这个好友\n");
                        else if(strcmp(USER.message_rece,"ns")==0)
                            printf("\t\t删除失败，系统里面没有你想删除的账号\n");
                        break;
                    }
                case 3://显示好友列表
                    {
                        printf("%s\n",USER.message_rece);
                        break;
                    }
                case 4://查询好友
                    {
                        if(strcmp(USER.message_rece,"yes")==0)
                        {
                            printf("\t\t该好友存在，并且在线\n");
                        }
                        else if(strcmp(USER.message_rece,"no")==0)
                        {
                            printf("\t\t该好友存在，并且不在线\n");
                        }
                        else if(strcmp(USER.message_rece,"ns")==0)
                        {
                            printf("\t\t该用户还不是您的好友\n");
                        }
                        else if(strcmp(USER.message_rece,"os")==0)
                        {
                            printf("\t\t该用户没有注册过\n");
                        }
                        break;
                    }
                case 5://私聊
                    {
                        printf("%s\n",USER.othername);
                        printf("%s",USER.sendtime);
                        printf("%s\n",USER.message);
                        break;
                    }
                case 6://查好友聊天记录
                    {
                        if(strcmp(USER.message_rece,"no")==0)
                            printf("\t\t你们还没有聊天记录\n");
                        else
                        {
                            printf("%s\n",USER.message_rece);
                        }
                        break;
                    }
                case 7://创建群
                    {
                        if(strcmp(USER.message_rece,"no")==0)
                            printf("\t\t群名称重复了！\n");
                        else if(strcmp(USER.message_rece,"yes")==0)
                        {
                            printf("\t\t%s群创建成功！群主是%s，快来邀请成员加群把！\n",USER.gpname,USER.uname);
                        }
                        break;
                    }
                case 8://解散群
                    {
                        if(strcmp(USER.message_rece,"ns")==0)
                            printf("\t\t没有这个群！\n");
                        else if(strcmp(USER.message_rece,"no")==0)
                            printf("\t\t你不是群主，不能解散该群!\n");
                        else if(strcmp(USER.message_rece,"yes")==0)
                            printf("\t\t解散成功\n");
                        break;
                    }
                case 9://退出群
                    {
                        if(strcmp(USER.message_rece,"ns")==0)
                            printf("\t\t你是群主，如果想要退出群，请选择解散群，谨慎操作哦～\n");
                        else if(strcmp(USER.message_rece,"no")==0)
                            printf("\t\t你不在这个群里面，是不是输入错误？\n");
                        else if(strcmp(USER.message_rece,"yo")==0)
                            printf("\t\t没有这个群\n");
                        else if(strcmp(USER.message_rece,"yes")==0)
                            printf("\t\t退群成功！\n");
                        break;
                    }
                case 10://查看群聊天记录
                    {
                        if(strcmp(USER.message_rece,"ns")==0)
                            printf("\t\t没有这个群！\n");
                        else if(strcmp(USER.message_rece,"no")==0)
                            printf("\t\t你不在这个群里面！\n");
                        else
                        {
                            printf("\t\t%s的聊天记录：\n",USER.gpname);
                            printf("%s\n",USER.message_rece);
                        }
                        break;
                    }
                case 11://显示群列表
                    {
                        printf("%s",USER.message_rece);
                        break;
                    }
                case 12://显示群成员
                    {
                        if(strcmp(USER.message_rece,"ns")==0)
                            printf("\t\t没有这个群！\n");
                        else if(strcmp(USER.message_rece,"no")==0)
                            printf("\t\t你不是这个群的成员，你不能查看该群成员！\n");
                        else
                            printf("%s",USER.message_rece);
                        break;
                    }
                case 13://群聊
                    {
                        printf("%s\n",USER.othername);
                        printf("%s\n",USER.sendtime);
                        printf("%s\n",USER.message_rece);
                        break;
                    }
                case 14://添加群
                    {
                        if(strcmp(USER.message_rece,"ns")==0)
                            printf("\t\t没有这个群！\n");
                        else if(strcmp(USER.message_rece,"no")==0)
                            printf("\t\t你已经是群成员了！\n");
                        else if(strcmp(USER.message_rece,"yes")==0)
                            printf("\t\t请求发送，等待处理！\n");
                        break;
                    }
                case 15://离线请求
                    {
                        printf("%s\n",USER.message_rece);
                        break;
                    }
                case 16://离线消息
                    {
                        if(strcmp(USER.message_rece,"no")==0)
                            printf("\t\t没有消息\n");
                        else
                        {
                            printf("%s\n",USER.message_rece);
                        }
                        break;
                    }
                case 17:
                    {
                        int fd;
                        if((fd=open(USER.filename,O_CREAT|O_RDWR|O_APPEND,0644))==-1)
                        {
                            my_err("open",__LINE__);
                        }
                        if(write(fd,USER.message,sizeof(USER.message))==-1)
                        {
                            my_err("write",__LINE__);
                        }
                        close(fd);
                        if(USER.flag2==1)
                        {
                            printf("\t\t文件接收成功!\n");
                            break;
                        }
                        break;
                    }
                case 18:
                    {
                        printf("%s",USER.message_rece);
                        break;
                    }
            }
        }
    }
}

void handle_connection(int sockfd)
{
    struct stu STU;
    pthread_t thid;
    char ptr[20];//记录现在登录的用户
    int choice1;//登录注册页面的选项
    int choice3;//用户登录后的主页面
    do
    {
        choice1=menu_log();
        switch(choice1)
        {
            case 1://登录
                {
                    STU=user_login(sockfd);
                    if(STU.stu_choice==1)//登陆成功
                    {
                        printf("\t\t登陆成功！\n");
                        system("clear");
                        if(pthread_create(&thid,NULL,(void *)my_recv,(void *)&sockfd)!=0)
                        {
                            my_err("pthread_create",__LINE__);
                        }
                        strcpy(ptr,STU.stu_name);
                        do
                        {
                            sleep(2);
                            system("clear");
                            choice3=menu_choice();
                            switch(choice3)
                            {
                                case 1://添加好友 要征得对方同意
                                    {
                                        add_friends(sockfd,&ptr);
                                        break;
                                    }
                                case 2://删除好友
                                    {
                                        del_friends(sockfd,&ptr);
                                        break;
                                    }
                                case 3://显示好友列表
                                    {
                                        display_friends(sockfd,&ptr);
                                        break;
                                    }
                                case 4://查询好友
                                    {
                                        find_friends(sockfd,&ptr);
                                        break;
                                    }
                                case 5://私聊
                                    {
                                        chat_friend(sockfd,&ptr);
                                        break;
                                    }
                                case 6://查聊天记录
                                    {
                                        find_chat(sockfd,&ptr);
                                        break;
                                    }
                                case 7://创建群
                                    {
                                        create_group(sockfd,&ptr);
                                        break;
                                    }
                                case 8://解散群
                                    {
                                        drop_group(sockfd,&ptr);
                                        break;
                                    }
                                case 9://退出群
                                    {
                                        bye_group(sockfd,&ptr);
                                        break;
                                    }
                                case 10://查看群聊天记录
                                    {
                                        find_group(sockfd,&ptr);
                                        break;
                                    }
                                case 11://显示已加群列表
                                    {
                                        show_group(sockfd,&ptr);
                                        break;
                                    }
                                case 12://显示群成员
                                    {
                                        show_group_user(sockfd,&ptr);
                                        break;
                                    }
                                case 13://群聊
                                    {
                                        chat_groups(sockfd,&ptr);
                                        break;
                                    }
                                case 14://添加群
                                    {
                                        add_group(sockfd,&ptr);
                                        break;
                                    }
                                case 15://查看离线请求
                                    {
                                        find_box(sockfd,&ptr);
                                        break;
                                    }
                                case 16://查看离线消息
                                    {
                                        find_box_message(sockfd,&ptr);
                                        break;
                                    }
                                case 17:
                                    {
                                        send_file(sockfd,&ptr);
                                        break;
                                    }
                                case 0://返回上一层
                                    {
                                        printf("\t\t退出chat\n");
                                        exit(1);
                                    }
                                default:
                                    {
                                        printf("\t\t输入格式不正确，请重新输入\n");
                                    }
                            }
                        }while(choice3!=0);
                    }
                    else if(STU.stu_choice==2)
                    {
                        printf("\t\t登录失败，密码输入错误\n");
                    }
                    else if(STU.stu_choice==3)
                    {
                        printf("\t\t登录失败，用户名输入错误\n");
                    }
                    break;
                }
            case 2://注册
                {
                    user_logout(sockfd);
                    break;
                }
            case 0:
                {
                      break;
                }
            default:
                {
                    printf("\t\t输入格式不正确，请重新输入\n");
                    break;
                }
        }
    }while(choice1!=0);
}

//menu_choice页面：
int menu_choice(void)
{
    sleep(1);
    char choice[30];
    memset(choice,0,sizeof(choice));
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
    printf("\t\t|             10.查看群聊天记录      |\n");
    printf("\t\t|             11.显示群列表          |\n");
    printf("\t\t|             12.显示群成员          |\n");
    printf("\t\t|             13.群聊                |\n");
    printf("\t\t|             14.添加群              |\n");
    printf("\t\t|             15.离线消息 请求       |\n");
    printf("\t\t|             16.离线消息 消息       |\n");
    printf("\t\t|             17.发送文件            |\n");
    printf("\t\t|             0.退出chat             |\n");
    printf("\t\t**************************************\n");
    printf("\t\t*请选择你的选项：");
    scanf("%[^\n]",choice);
    getchar();
    if((strcmp(choice,"0")==0)||(strcmp(choice,"1")==0)||(strcmp(choice,"2")==0)||(strcmp(choice,"3")==0)||(strcmp(choice,"4")==0)||(strcmp(choice,"5")==0)||(strcmp(choice,"6")==0)||(strcmp(choice,"7")==0)||(strcmp(choice,"8")==0)||(strcmp(choice,"9")==0))
        return atoi(choice);
    else if((strcmp(choice,"10")==0)||(strcmp(choice,"11")==0)||(strcmp(choice,"12")==0)||(strcmp(choice,"13")==0)||(strcmp(choice,"14")==0)||(strcmp(choice,"15")==0)||(strcmp(choice,"16")==0)||(strcmp(choice,"17")==0))
        return atoi(choice);
    else
        return 30;
}

//menu_log页面：
int menu_log(void)
{
    char choice[30];
    memset(choice,0,sizeof(choice));
    printf("\t\t**************************************\n");
    printf("\t\t|             欢迎来到chat           |\n");
    printf("\t\t|             1.登录                 |\n");
    printf("\t\t|             2.注册                 |\n");
    printf("\t\t|             0.退出chat             |\n");
    printf("\t\t**************************************\n");
    printf("\t\t请输入您的选择:");
    scanf("%[^\n]",choice);
    getchar();
    if((strcmp(choice,"0")==0)||(strcmp(choice,"1")==0)||(strcmp(choice,"2")==0))
        return choice[0]-'0';
    else
        return 5;
}

struct stu user_login(int sockfd)//登录
{
    struct stu STU;
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    printf("\t\t* 请输入用户名：");
    scanf("%[^\n]",USER.uname);
    getchar();
    printf("\t\t* 请输入密码：");
    scanf("%[^\n]",USER.upassword);
    getchar();
    USER.flag=1;
    USER.sd=sockfd;
    if(send(sockfd,&USER,sizeof(struct use),0)==-1)
    {
        my_err("send",__LINE__);
    }
    if(recv(sockfd,&USER,sizeof(struct use),0)==-1)
    {
        my_err("recv",__LINE__);
    }
    if(strcmp(USER.message_rece,"yes")==0)
    {
        STU.stu_choice=1;
        strcpy(STU.stu_name,USER.uname);
        return STU;
    }
    else if(strcmp(USER.message_rece,"ns")==0)
    {
        STU.stu_choice=2;
        strcpy(STU.stu_name,USER.uname);
        return STU;
    }
    else if(strcmp(USER.message_rece,"no")==0)
    {
        STU.stu_choice=3;
        strcpy(STU.stu_name,USER.uname);
        return STU;
    }
}

void user_logout(int sockfd)//注册
{  
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    printf("\t\t* 请输入用户名：");
    scanf("%[^\n]",USER.uname);
    getchar(); 
    printf("\t\t* 请输入密码：");
    scanf("%[^\n]",USER.upassword);
    getchar();
    USER.flag=2;
    USER.sd=sockfd;
    if(send(sockfd,&USER,sizeof(struct use),0)==-1)
    {
        my_err("send",__LINE__);
    }
    if(recv(sockfd,&USER,sizeof(USER),0)<0)
    {
        my_err("recv",__LINE__);
    }
    if(strcmp(USER.message_rece,"yes")==0)//成功
        printf("\t\t注册成功！\n");
    else if(strcmp(USER.message_rece,"no")==0)
        printf("\t\t用户名称已被注册，注册失败\n");
}

void add_friends(int sockfd,char*ptr)
{
    char ch;
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    USER.sd=sockfd;
    strcpy(USER.uname,ptr);
    printf("\t\t请输入想要加的好友昵称：");
    scanf("%[^\n]",USER.othername);
    getchar();
    printf("\t\t请输入备注消息：");
    scanf("%[^\n]",USER.message);
    getchar();
    USER.flag=3;
    printf("\t\t请求确认是否添加？(确认：y/Y 不确认：n/N)");
    scanf("%c",&ch);
    getchar();
    if((ch=='y')||(ch=='Y'))
    {
        if(send(sockfd,&USER,sizeof(USER),0)==-1)
        {
            my_err("send",__LINE__);
        }
    }
    else
    {
        printf("\t\t你已经取消了操作！\n");
    }
}

void del_friends(int sockfd,char *ptr)
{
    char ch;
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    USER.sd=sockfd;
    strcpy(USER.uname,ptr);
    printf("\t\t请输入想要删除的好友昵称：");
    scanf("%[^\n]",USER.othername);
    getchar();
    USER.flag=4;
    printf("\t\t是否删除这个好友? (确认:y/Y 不确认:n/N)");
    scanf("%c",&ch);
    getchar();
    if((ch=='y')||(ch=='Y'))
    {
        if(send(sockfd,&USER,sizeof(USER),0)==-1)
        {
            my_err("send",__LINE__);
        }
    }
    else
    {
        printf("\t\t你已经取消操作\n");
    }
}

void display_friends(int sockfd,char*ptr)
{
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    USER.sd=sockfd;
    strcpy(USER.uname,ptr);
    USER.flag=5;
    if(send(sockfd,&USER,sizeof(struct use),0)==-1)
    {
        my_err("send",__LINE__);
    }
}

void find_friends(int sockfd,char *ptr)
{
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    USER.sd=sockfd;
    strcpy(USER.uname,ptr);
    USER.flag=6;
    printf("\t\t请输入想要查询的好友名称：");
    scanf("%[^\n]",USER.othername);
    getchar();
    if(send(sockfd,&USER,sizeof(struct use),0)==-1)
    {
        my_err("send",__LINE__);
    }
}

void find_chat(int sockfd,char*ptr)
{
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    USER.sd=sockfd;
    strcpy(USER.uname,ptr);
    USER.flag=8;
    printf("\t\t请输入你想查找聊天的对象：");
    scanf("%[^\n]",USER.othername);
    getchar();
    if(send(sockfd,&USER,sizeof(struct use),0)==-1)
    {
        my_err("send",__LINE__);
    }
}

void create_group(int sockfd,char*ptr)
{
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    USER.sd=sockfd;
    strcpy(USER.uname,ptr);
    USER.flag=9;
    printf("\t\t请输入你想创建的群名：");
    scanf("%[^\n]",USER.gpname);
    getchar();
    if(send(sockfd,&USER,sizeof(struct use),0)==-1)
    {
        my_err("send",__LINE__);
    }
}

void drop_group(int sockfd,char*ptr)
{
    char ch;
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    USER.sd=sockfd;
    strcpy(USER.uname,ptr);
    USER.flag=10;
    printf("\t\t请输入想要解散的群：");
    scanf("%[^\n]",USER.gpname);
    getchar();
    printf("\t\t是否解散该群？(确认：y/Y,不确认：n/N)");
    scanf("%c",&ch);
    getchar();
    if((ch=='y')||(ch=='Y'))
    {
        if(send(sockfd,&USER,sizeof(struct use),0)==-1)
        {
            my_err("send",__LINE__);
        }    
    }
    else
    {
        printf("\t\t已经取消了操作！\n");
    }
}

void bye_group(int sockfd,char*ptr)
{
    char ch;
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    USER.sd=sockfd;
    strcpy(USER.uname,ptr);
    USER.flag=11;
    printf("\t\t请输入你想要退出的群：");
    scanf("%[^\n]",USER.gpname);
    getchar();
    printf("\t\t是否要退出该群？(确认：y/Y,不确认：n/N)");
    scanf("%c",&ch);
    getchar();
    if((ch=='y')||(ch=='Y'))
    {
        if(send(sockfd,&USER,sizeof(struct use),0)==-1)
        {
            my_err("send",__LINE__);
        }
    }
    else
    {
        printf("\t\t已经取消了操作！\n");
    }
}

void find_group(int sockfd,char*ptr)
{
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    USER.sd=sockfd;
    strcpy(USER.uname,ptr);
    USER.flag=12;
    printf("\t\t请输入你想要查找的群记录的群名称：");
    scanf("%[^\n]",USER.gpname);
    getchar();
    if(send(sockfd,&USER,sizeof(struct use),0)==-1)
    {
        my_err("send",__LINE__);
    }
}

void show_group(int sockfd,char*ptr)
{
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    USER.fd=sockfd;
    strcpy(USER.uname,ptr);
    USER.flag=13;
    if(send(sockfd,&USER,sizeof(struct use),0)==-1)
    {
        my_err("send",__LINE__);
    }
}

void show_group_user(int sockfd,char*ptr)
{
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    USER.fd=sockfd;
    strcpy(USER.uname,ptr);
    USER.flag=14;
    printf("\t\t请输入你要查询的群：");
    scanf("%[^\n]",USER.gpname);
    getchar();
    if(send(sockfd,&USER,sizeof(struct use),0)==-1)
    {
        my_err("send",__LINE__);
    }
}

void find_box(int sockfd,char*ptr)
{
    int choice;
    char choice1;
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    USER.sd=sockfd;
    strcpy(USER.uname,ptr);
    USER.flag=17;
    if(send(sockfd,&USER,sizeof(struct use),0)==-1)
    {
        my_err("send",__LINE__);
    }
    sleep(1);
    while(1)
    {
        printf("\t\t请选择序号：");
        scanf("%d",&choice);
        getchar();
        if(choice==0)
        {
            printf("\t\t已经退出\n");
            break;
        }
        printf("\t\t请再次确认：(y(Y)/n(N))");
        scanf("%c",&choice1);
        getchar();
        if((choice1=='y')||(choice1=='Y'))
        {
            USER.flag=19;
            USER.back=choice;
            if(send(USER.sd,&USER,sizeof(USER),0)==-1)
            {
                my_err("send",__LINE__);
            }
            break;
        }
        else
        {
            printf("\t\t你已经取消了操作\n");
            break;
        }
    }
}

void find_box_message(int sockfd,char*ptr)
{
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    USER.fd=sockfd;
    strcpy(USER.uname,ptr);
    USER.flag=18;
    if(send(sockfd,&USER,sizeof(struct use),0)==-1)
    {
        my_err("send",__LINE__);
    }
}

void add_group(int sockfd,char*ptr)
{
    char ch;
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    USER.fd=sockfd;
    strcpy(USER.uname,ptr);
    USER.flag=16;
    printf("\t\t请输入群名称：");
    scanf("%[^\n]",USER.gpname);
    getchar();
    printf("\t\t请输入备注消息：");
    scanf("%[^\n]",USER.message);
    getchar();
    printf("\t\t请求确认是否添加？(确认：y/Y 不确认：n/N)");
    scanf("%c",&ch);
    getchar();
    if((ch=='y')||(ch=='Y'))
    {
        if(send(sockfd,&USER,sizeof(USER),0)==-1)
        {
            my_err("send",__LINE__);
        }
    }
    else
    {
        printf("\t\t你已经取消了操作！\n");
    }
}

void chat_friend(int sockfd,char *ptr)
{
    struct use USER;
    time_t t;
    memset(&USER,0,sizeof(struct use));
    USER.fd=sockfd;
    strcpy(USER.uname,ptr);
    USER.flag=7;
    printf("请输入聊天的用户名:");
    scanf("%[^\n]",USER.othername);
    getchar();
    while(1)
    {
        time(&t);
        strcpy(USER.sendtime,ctime(&t));
        scanf("%[^\n]",USER.message);
        getchar();
        if(strcmp(USER.message,"quit")==0)
            break;
        if(send(sockfd,&USER,sizeof(USER),0)==-1)
        {
            my_err("send",__LINE__);
        }
    }
}

void chat_groups(int sockfd,char*ptr)
{
    struct use USER;
    time_t t;
    memset(&USER,0,sizeof(struct use));
    USER.fd=sockfd;
    strcpy(USER.uname,ptr);
    USER.flag=15;
    printf("\t\t请输入群名称：");
    scanf("%[^\n]",USER.gpname);
    getchar();
    while(1)
    {
        time(&t);
        strcpy(USER.sendtime,ctime(&t));
        scanf("%[^\n]",USER.message);
        getchar();
        if(strcmp(USER.message,"quit")==0)
            break;
        if(send(sockfd,&USER,sizeof(USER),0)==-1)
        {
            my_err("send",__LINE__);
        }
    }
}

void send_file(int sockfd,char*ptr)
{
    int fd;
    int len;
    int len2=0;
    struct use USER;
    memset(&USER,0,sizeof(struct use));
    USER.fd=sockfd;
    strcpy(USER.uname,ptr);
    USER.flag=21;
    printf("\t\t请输入发送的好友：");
    scanf("%[^\n]",USER.othername);
    getchar();
    printf("\t\t请输入文件+路径名：");
    scanf("%[^\n]",USER.path);
    getchar();
    printf("\t\t再次输入文件名称：");
    scanf("%[^\n]",USER.filename);
    getchar();
    if((fd=open(USER.path,O_RDONLY))==-1)
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
        if(send(USER.fd,&USER,sizeof(USER),0)==-1)
        {
            my_err("send",__LINE__);
        }
    }
}
