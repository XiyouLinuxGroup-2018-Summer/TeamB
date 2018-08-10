#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main()
{
    char buffer[32];
    int ret = 0;
    int host = 0;
    int network = 0;
    unsigned int adress = 0;
    struct in_addr in;
    in.s_addr = 0;

    /*输入一个以"."为分隔的字符串形成的IP地址*/
    printf("please input your ip adress:");
        fgets(buffer,31,stdin);
    buffer[31]='\0';

    /*示例使用inet_aton函数()*/ //转换为二进制的网络字节顺序的IP地址
    if(inet_aton(buffer,&in) == 0)
        printf("inet_aton: \t invalid adress\n");
    else
        printf("inet_aton:\t0x%x\n",in.s_addr);

    /*示例使用inet_addr函数()*/
    if((adress = inet_addr(buffer)) == INADDR_NONE)
        printf("inet_addr: \t invalid adress\n");
    else
        printf("inet_addr:\t0x%x\n",adress);
    
    /*示例使用inet_network()函数*/  //转换为二进制的主机字节顺序形成的IP地址
    if((adress = inet_network(buffer)) == -1)
        printf("inet_network: \t invalid adress\n");
    else
    printf("inet_network:\t0x%x\n",adress);

    /*示例使用inet_ntoa()函数*/    //将值为in的网络字节顺序形式的二进制IP地址转换成以"."分隔的字符串形式
    if(inet_ntoa(in) == NULL)
        printf("inet_ntoa: \t invalid adress\n");
    else
        printf("inet_ntoa:\t%s\n",inet_ntoa(in));
    
    /*示例使用inet_lnaof()函数和 inet_netof()函数*/
    host = inet_lnaof(in); //提取出主机地址
    network = inet_netof(in); //提取出网络地址
    printf("inet_lnaof:\t0x%x\n",host);
    printf("inet_netof:\t0x%x\n",network);

    in = inet_makeaddr(network , host);
    printf("inet_makeaddr:\t0x%x\n",in.s_addr);
    
}