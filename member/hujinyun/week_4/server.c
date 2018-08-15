#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <ctype.h> //含toupper小写字母转化为大写字母
#define MAXLINE 255
#define SERV_PORT 8000

int main(int argc, char **argv)
{
    int sfd,cfd;
    sfd= socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serv_addr,client_addr;
    int len,i;
    socklen_t addr_len;

    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(SERV_PORT);
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);

    if(bind(sfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {//绑到本地某一端口上
        perror("bind");
        exit(1);
    }
    listen(sfd,128);//设置listen
    printf("wait for connect----------\n");
    addr_len = sizeof(client_addr);
    cfd = accept(sfd,(struct sockaddr *)&client_addr, &addr_len);
    //第二个参数保存发送连接请求的主机和端口
    //返回新新的代表客户端的套接字,进行可以利用这个新套接字与与客户端交换数据
    if(cfd == -1)
    {
        perror("accept");
        exit(1);
    }
    char buf[256];
    
    printf("client IP :%s %d\n",                                                                    
            inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,buf,sizeof(buf)),                        
          ntohs(client_addr.sin_port)); 
    while(1)
    {
        len = recv(cfd,buf,sizeof(buf),0);//读取客户端数据
        if(len == -1)
        {
            perror("recv");
            exit(1);
        }
        if(write(STDOUT_FILENO,buf,len)<0)//输出到客户端屏幕
        {
            perror("send屏幕");
            exit(1);
        }
        for(i=0; i<len; i++)
        buf[i]=toupper(buf[i]); //大写转换来区分下发回数据
        if(send(cfd,buf,len,0)<0)//写数据到客户端
        {
            perror("write");
            exit(1);
        }

       
    }
    close(sfd);
    close(cfd);
    return 0;
        
}

/*sever.c*/
/*#include <sys/types.h>                                                                              
#include <sys/socket.h>                                                                             
#include <strings.h>                                                                                
#include <ctype.h>                                                                                  
#include <stdio.h>                                                                                  
#include <stdlib.h>                                                                                 
#include <unistd.h>                                                                                 
#include <arpa/inet.h>                                                                              
#define SERV_PORT 8000                                                                              
void perr_exit(const char *str)                                                                     
{                                                                                                   
    perror(str);                                                                                    
    exit(1);                                                                                        
}                                                                                                   
int main()                                                                                          
{                                                                                                   
    int sfd = socket(AF_INET,SOCK_STREAM,0),cfd;                                                    
    struct sockaddr_in servaddr;                                                                    
    struct sockaddr_in client_addr;                                                                 
    int i,len;                                                                                      
    socklen_t addr_len;                                                                             
    //init                                                                                          
    bzero(&servaddr,sizeof(struct sockaddr_in));                                                    
    servaddr.sin_family = AF_INET;                                                                  

    //htons htonl 都属于网络字节序转换，在代码段之后会进行解释，就先理解为转换为网络中所需要的类型  
    servaddr.sin_port = htons(SERV_PORT);                                                           
    //INADDR_ANY表示任意都可连接（因为客户端不是来自同一个网络地址）                                
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//                                                 
    if(bind(sfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)                                 
        perr_exit("bind error");                                                                    

    //设置可连接数为128                                                                             
    listen(sfd,128);                                                                                

    printf("wait for conncet---------\n");                                                          
    addr_len = sizeof(client_addr);                                                                 
    cfd = accept(sfd,(struct sockaddr *)&client_addr,&addr_len);                                    
    //第二个参数保存发送连接请求的主机地址和端口
    if(cfd == -1)                                                                                   
        perr_exit("accept error");                                                                  
    char buf[256];                                                                                  

    /*系统还为我们封装了IP地址转换函数                                                              
     * 因为IP地址在网络中为网络字节序二进制值，而我们平常使用的是ASCII字符串，                      
     * 故有这一组函数来进行转换，其实也不难记，可以这样形式的记住，以免用混ip to net,net to ip      
     * #include <arpa/inet.h>                                                                       
     * int inet_pton(int af, const char *src, void *dst);                                           
     * const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);                   
     * */                                                                                           
/*printf("client IP :%s %d\n",                                                                    
            inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,buf,sizeof(buf)),                        
          ntohs(client_addr.sin_port));                                                             
    while(1)                                                                                        
    {                                                                                               
        len = read(cfd,buf,sizeof(buf)); //读取客户端的数据                                         
        if(len == -1)                                                                               
            perr_exit("read error");*/
       /* 
          if(len == 0)                                                                                
        {                                                                                           
            printf("the other size closed\n");                                                             
            close(cfd);                                                                             
            close(sfd);                                                                             
            exit(1);                                                                                
        }           
            */                                                            
 /*       if(write(STDOUT_FILENO,buf,len) < 0) //输出到屏幕                                           
            perr_exit("write error");                                                               
        for(i = 0 ;i < len ; i++)   //进行大写转换                                                  
            buf[i] = toupper(buf[i]);                                                               
        if(write(cfd,buf,len) < 0) //写数据到客户端                                                 
            perr_exit("write error");                                                               
    }                                                                                               
    //关闭打开的文件描述符，虽然不会执行到这里往下的部分，但要养成良好习惯，打开文件描述符，用完    
    //就要关闭                                                                                      
    close(sfd);                                                                                     
    close(cfd);                                                                                     
    return 0;                                                                                       
}         

*/