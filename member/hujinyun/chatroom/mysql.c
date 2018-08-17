#include<stdio.h>
#include<mysql/mysql.h>
#include<string.h>
int main(){
	MYSQL mysql;
	//初始化MYSQL句柄
	if(NULL == mysql_init(&mysql)){
		printf("init: %s\n",mysql_error(&mysql));
	}

	//建立MYSQL连接
	
	if(NULL == mysql_real_connect(&mysql,"localhost","root","199951k","STUDENT",0,NULL,0)){
		printf("connect: %s\n",mysql_error(&mysql));
	}
    else
        printf("connect to Mysql successfully!\n");

	
	//执行插入语句???
	char *sql = "INSERT INTO STU(no,s_name) VALUES('04163002','fujie')";
	if(0 != mysql_real_query(&mysql,sql,strlen(sql))){
		printf("query: %s\n",mysql_error(&mysql));
	}//字符串包含二进制数据,就必须使用my_real_query查询

	//执行查询语句
	sql = "SELECT no,s_name FROM STU";
	if(0 != mysql_real_query(&mysql,sql,strlen(sql))){
		printf("query: %s\n",mysql_error(&mysql));
	}
	MYSQL_RES *res = mysql_store_result(&mysql);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(res))) {
		printf("%s ******** %s******* \n",row[0],row[1]);
	}
	mysql_free_result(res);
	mysql_close(&mysql);
}
