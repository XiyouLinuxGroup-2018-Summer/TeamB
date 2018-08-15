#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#define MAXLINE 255
#define SERV_PORT 8000

void perr_exit(const char *str)
{
    perror(str);
    exit(1);
}
int main(int argc, char **argv)
{
    int sfd;
    struct sockaddr_in serv_addr;
    int len;
    char buf[MAXLINE];
    sfd = socket(AF_INET, SOCK_STREAM,0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,argv[1],&serv_addr.sin_addr.s_addr); //将char *型IP地址转换为网络二进制序

    if( connect(sfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0)
    {
        perr_exit("connect");
    }
    while(fgets(buf,sizeof(buf), stdin))
    {
        if(send(sfd,buf,strlen(buf),0)<0)//写数据到服务器
            perr_exit("send38");
        len = recv(sfd, buf, sizeof(buf),0);//读取服务器传递的数据
        if(len <0)
            perr_exit("read error");
        if(len ==0)
        {
            printf("the other size closed\n");
            close(sfd);
            exit(1);
        }
        if(write(STDOUT_FILENO,buf,len)< 0)/*!!!!!!!!!!!!!!之前一直输出显示write error: Socket operation on non-socket
题好像出在这里,应该是输出到屏幕不能用send,要用write*/
            perr_exit("write error");
    }
    close(sfd);
    return 0;
}
