/*************************************************************************
	> File Name: test_address.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月08日 星期三 14时59分21秒
 ************************************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
int main(void)
{
    char buffer[32];
    int ret=0;
    int host=0;
    int network=0;
    unsigned int address=0;
    struct in_addr in;
    in.s_addr=0;
    /*输入一个以"."分割的字符串形式的IP地址*/
    printf("please input your ip address:");
    fgets(buffer,31,stdin);
    buffer[31]='\0';
    /*实例使用inet_aton()函数*/
    if((ret=inet_aton(buffer,&in))==0)
        printf("inet_aton:\tinvalid address\n");
    else
        printf("inet_aton:\t0x%x\n",in.s_addr);
    /*实例使用inet_addr()函数*/
    if((address=inet_addr(buffer))==INADDR_NONE)
        printf("inet_addr:\tinvalid address\n");
    else
        printf("inet_addr:\t0x%x\n",address);
    /*实例使用inet_network函数*/
    if((address=inet_network(buffer))==-1)
        printf("inet_network:\tinvalid address\n");
    else
        printf("inet_work:\t0x%x\n",address);
    /*实例使用inet_ntoa函数*/
    if(inet_ntoa(in)==NULL)
        printf("inet_ntoa:\tinvalid address\n");
    else
        printf("inet_ntoa:\t%s\n",inet_ntoa(in));
    /*实例使用inet_lnaof()和inet_netof()函数*/
    host=inet_lnaof(in);
    network=inet_netof(in);
    printf("inet_inaof:\t0x%x\n",host);
    printf("inet_netof:\t0x%x\n",network);
    in=inet_makeaddr(network,host);
    printf("inet_makeaddr:0x%x\n",in.s_addr);
    return 0;
}
