最开始的学习疑问,网络编程上的这些乱七八糟的名词这些都是用来干什么的?

地址:为了使网络上的计算机通过唯一标识进行相互间通信
端口:解决多个程序要同时访问网络
IP协议:确定到达目的地的路由
UDP协议:向应用程序提供一种面向无连接的服务
TCP协议:提供一种面向连接的,可靠的数据传输服务

套接字是什么
在两个程序进行通讯连接的一个端点.是连接应用程序和网络驱动程序的桥梁.

网络应用程序通过socket进行数据的发送与接收过程:
> socket在应用程序中创建,通过绑定与网络驱动建立关系.此后,应用程序送给socket的数据,由socket交给网络驱动程序向网络上发送出去.计算机从网络上收到与该socket绑定IP地址和端口号相关的数据之后,由网络驱动交给socket,应用程序便可以从该socket中提取接收到的数据.


假设第二个程序被加入的网络的Host B中，那么由Host A传来的信息如何能被正确的传给程序B而不是传给新加入的程序呢？这是因为**每一个基于TCP/IP网络通讯的程序都被赋予了唯一的端口和端口号，端口是一个信息缓冲区，用于保留Socket中的输入/输出信息**，端口号是一个16位无符号整数，范围是0-65535，以区别主机上的每一个程序（端口号就像房屋中的房间号），低于256的端口号保留给标准应用程序，比如pop3的端口号就是110，**每一个套接字都组合进了IP地址、端口、端口号，这样形成的整体就可以区别每一个套接字。**

###创建并使用一套套接字的流程:
创建套接字:`socket函数`
(所有端)
```
int sock_fd;
sock_fd = socket(AF_INET, SOCK_STREAM,0);/*创建TCP套接字*/
//sock_fd = socket(AF_INET,SOCK_DRGAM,0);/*创建UDP套接字*/
if (sock_fd<0)
{
    perror("socket");
    exit(1);
}
```
建立连接:`connect函数`
*指定套接字上建立连接*
(针对客户端)
```
struct sockaddr_in serv_addr; /*指定服务器IP地址和端口号的结构体*/
memset(&serv_addr,0,sizeof(sockaddr_in))/*将serv_addr各个字段清零*/
serv_addr.sin_family = AF_INET; /*使用IPv4 TCP/IP协议*/
serv_addr.sin_port = htons(80); /*htons是字节顺序转换函数. host to network short 将主机unsigned short 型数据转化成网络字节顺序*/
if(inet_aton("172.17.242.131",&serv_addr.sin_addr)<0)/*inet_aton函数将一个字符串转化成一个网络地址,并把该网络地址赋给第二个参数*/
{
    perror("inet_aton");
    exit(1);
}
/*使用sock_fd套接字连接到serv_addr指定的目的地址上,假定sock_fd已定义为前提*/
if(connect(sock_fd, (struct sockedaddr *)&serv_addr, sizeof(struct sockaddr_in))<0)
{
    perror("connect");
    exit(1);
}//注意serv_addr强制类型转换为struct sockaddr类型


```
绑定套接字:`bind`
*将一个套接字文件描述符与一个本地地址绑定在一起，即把发送数据的端口地址和IP地址进行了指定。*
(针对服务端)
```
struct sockaddr_in serv_addr;
mumset(&serv_addr, 0, sizeof(struct sockaddr_in));
serv_addr.sin_family AF_INET;
serv_addr.port_port = htons(80);
serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);/*INADDR_ANY表示本服务器程序将处理来自所有网络接口上相应端口的连接请求 ,(对于只有一个IP地址的计算机,INADDR_ANY对应就是它的IP地址)*/
//htonl表示将主机unsigned int型数据转换成网络字节顺序
if(bind(sock_fd, (struct socketaddr *)&serv),sizeof(struct sckaddr_in)<0)
{
    perror("bid");
    exit(1);
}
``` 
在套接字上监听:`listen`
*将该套接字转化为监听套接字(被动监听)*
(针对服务端)

*由于函数socket创建的套接字是主动套接字,这种套接字可以用来主动请求某个服务器(通过函数connect针对客户端)*
*作为服务器端的程序,通常在某个端口监听等待来自客户端的连接请求(bind)*
```

#define LISTEN_NUM 12 //定义连接请求队列长度
if(listen(sock_fd, LISTEN_N))UM)<0)
/*第二个参数表示 服务器端设置一个连接队列,记录已建立的连接(该服务器连接的多个客户端)的最长长度*/
{
    perror("listen");
    exit(1);
}
```
注:listen函数只是将套接字设置为倾听模式以等待连接请求,并不能接受连接请求.

接受连接`accept`
(用于服务器端)

```
/*套接字为阻塞方式下该函数的常见用法*/
int client_fd;
int client_len;
struct socketaddr_in client_addr;
...
client_len = sizeof(struct sockaddr_in);
client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_len);
/*accept执行成功返回一个新创建的套接字文件描述符.进程可以利用这个新的套接字描述符与客户端交换数据,原先套接字继续等待客户端的连接请求*/
if(client_fd < 0)
{
    perror("accept");
    exit(1);     
}
```

注意:对于客户端的 connect() 函数，该函数的功能为客户端主动连接服务器，建立连接是通过三次握手，而这个连接的过程是由内核完成，不是这个函数完成的，这个函数的作用仅仅是通知 Linux 内核，让 Linux 内核自动完成 TCP 三次握手连接，最后把连接的结果返回给这个函数的返回值（成功连接为0， 失败为-1）。

listen()函数不会阻塞，它主要做的事情为，将该套接字和套接字对应的连接队列长度告诉 Linux 内核，然后，listen()函数就结束。

这样的话，当有一个客户端主动连接（connect()），Linux 内核就自动完成TCP 三次握手，将建立好的链接自动存储到队列中，如此重复。


通常的情况，客户端的 connect() 函数默认会一直阻塞，直到三次握手成功或超时失败才返回（正常的情况，这个过程很快完成）。

所以，只要 TCP 服务器调用了 listen()，客户端就可以通过 connect() 和服务器建立连接，而这个连接的过程是由内核完成,不用担心具体函数实现三次握手的过程.
在使用TCP或UDP具体传送数据前的模型如下:
![基本模型][1]


  传送数据(发送与接受)有两种套接字传送方式:TCP和UDP
  那么在使用前想知道这两种的到底要什么场合使用呢优劣是什么呢?
  UDP:
 
  * 他是将应用程序发来数据在收到的那一刻,立刻按照原样发送到网上的一种机制. 
  * UDP一种利用IP提供面向无连接的通信服务.不提供复杂的控制机制
  * 丢包不重发,包到达顺序乱掉不纠正
  * 尽最大努力交付,不保证可靠交付
  * 不进行流量控制避免网络拥堵
  * 以上没有的细节控制,需要交给应用程序处理
  * 主要用于高速传输和实时性有较高要求的通信或广播通信.
  * UDP支持一对一,一对多,多对一和多对多的交互通信
  * 首部开销只有8字节

<br>
TCP:  

  * 充分体现了数据传输时各种控制功能
  * 可以进行丢包的重发控制,还可以对乱掉的分包进行顺序控制
  * 作为一种面向有连接的协议,只有确认通信对端存在时才会发送数据,从而可以控制流量的浪费
  * 可靠
  * 每一条TCP只能是点到点的
  * 首部开销20字节
 <br>
>举个栗子:IP电话,如果使用TCP,数据在传送的过程中若丢失就会被重发,但是这样就无法流畅地传输通话人的声音,也无法实时正常交流.而采用UDP,它不会进行重发处理.从而也不会有声音大幅度延迟到达的问题.即使有部分数据丢失,也只是影响了一小部分通话.
与此同理多播与广播也是用的是UDP而不是TCP.

<br>


TCP套接字的数据传输
发送数据:`send`
*函数send用来在TCP套接字上发送数据*
*只能对处于连接状态的套接字使用.(accept后)*
```
/*套接字为阻塞方式下,常见用法*/
#define BUFFERSIZE 1500
char send_buf[BUFFERSIZE];
...
if(send(client_fd, send_buf, len, 0)<0)
/*第二个数据为待发送数据的缓冲区,第三个数据为待发送数据的长度,保证数据长度的一致性*/
{
    perror("send");
    exit(1);
}
```
注:执行成功只是说明数据写入套接字的缓冲区,并不代表数据已经通过网络发送到目的地

接收数据`recv`
*函数recv用来在TCP套接字上接收数据*
*从面向连接的套接字上接收数据并保存到指定的缓存区*
```
char recvbuf[BUFFERSIZE];
if(recv(client_fd, recv_buf, sizeof(recv_buf),0)<0)
{
    perror("recv");
    exit(1);
}
```

上述两个函数具体在哪里用呢?
客户端发送,服务端接收,服务端对信息进行处理,再发送给客户端,客户端再接收.
每个TCP socket在内核中都有一个发送缓冲区和一个接收缓冲区.接受缓冲区把数据存入内核,,应用进程一直没有调用recv()进行读取的话,此数据会一致缓存在相应的socket的接受缓存区里.
也就是说,不管进程是否调用recv()读取socket，对端发来的数据都会经由内核接收并且缓存到socket的内核接收缓冲区之中。`recv()所做的工作，就是把内核缓冲区中的数据拷贝到应用层用户buffer里面,并返回,仅此而已`.
`进程调用send()发送数据时,一般情况就是,将数据拷贝进入拷贝进入socket内核发送缓冲之中`
注意:
1.send（）和recv（）没有固定的对应关系，不定数目的send()可以触发不定数目的recv（）,主要是发送至缓冲区和从缓冲区读,读几次,读到哪里(数组或屏幕都可以)都可以.
2.send（）只负责拷贝，拷贝到内核就返回
3.c/s数据处理如下:
![c/s数据处理][2]

补充:
在TCP连接中，recv等函数默认为阻塞模式(block)，即直到有数据到来之前函数不会返回，而我们有时则需要一种超时机制使其在一定时间后返回而不管是否有数据到来，这里我们就会用到`setsockopt()`函数：

    int  setsockopt(int  s, int level, int optname, void* optval, socklen_t* optlen);
    这里我们要涉及到一个结构：
    struct timeval
    {
            time_t tv_sec;
            time_t tv_usec;
    };
    这里第一个域的单位为秒，第二个域的单位为微秒。
    struct timeval tv_out;
    tv_out.tv_sec = 1;
    tv_out.tv_usec = 0;
    填充这个结构后，我们就可以以如下的方式调用这个函数：
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));(具体参数可以man一下，或查看MSDN)
    这样我们就设定了recv()函数的超时机制，当超过tv_out设定的时间而没有数据到来时recv()就会返回0值。
    
附上上述实例,基于单线程的简单的c/s模型
```
/*server*/
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
    //INADDR_ANY表示任意都可连接（因为客户端不是来自同一个网络地址）                                

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
```
----------
```
/*client*/
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

```

  [1]: https://img-blog.csdn.net/20180813214053176?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tra2tkZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70
  [2]: https://img-blog.csdn.net/20180815090728881?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tra2tkZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70