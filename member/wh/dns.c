#include<netdb.h>
#include<string.h>
#include<stdio.h>
#include<arpa/inet.h>
int main(int argc,char *argv[])
{	
	char host[] = "www.douyu.com";		//将要查询的主机域名
	struct hostent *ht = NULL;
	char str[30];
	ht = gethostbyname(host);				//查询主机www.baidu.com.cn
	
	//打印主机相关信息
	if(ht) {
		int i = 0;
		printf("get the host :%s addr\n",host);	//原始域名
		printf("name:%s\n",ht->h_name);

		printf("type:%s\n",ht->h_addrtype == AF_INET?"AF_INET":"AF_INET6");
		printf("length:%d\n",ht->h_length);		//IP地址长度
		//打印IP地址
		for(i = 0;;i++) {
			if(ht->h_addr_list[i] != NULL)		//不是IP地址数组的结尾
				printf("IP:%s\n",inet_ntop(ht->h_addrtype,ht->h_addr_list[i],str,30));	//打印IP地址
			else
				break;
		}

		//打印域名地址
		for(i=0;;i++) {
			if(ht->h_aliases[i] != NULL)
				printf("alias %d:%s\n",i,ht->h_aliases[i]);
			else
				break;
		}
	}
	else
		puts("no 网络");
	return 0;
}
