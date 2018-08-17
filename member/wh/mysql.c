#include<stdio.h>
#include<mysql.h>
#include<string.h>
int main(int argc,char *argv[])
{
	MYSQL mysql;
	MYSQL_RES	*res = NULL;
	MYSQL_ROW	row;
	char *query_str = NULL;
	int rc,i,fields;
	int rows;
	if(NULL == mysql_init(&mysql)) {			//分配和初始化mysql对象
		printf("mysql_init():%s\n",mysql_error(&mysql));
		return -1;
	}if(NULL == mysql_real_connect(&mysql,NULL,"root","123456","wh",0,NULL,0)) {
		printf("mysql_real_cpnnect():%s\n",mysql_error(&mysql));
		return -1;
	}
	printf("connected Mysql successful\n");
	//查询数据库示例
	query_str = "select * from info";
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(rc != 0) {
		printf("mysql_real_query():%s\n",mysql_error(&mysql));
		return -1;
	}
	res = mysql_store_result(&mysql);
	if(res == NULL) {
		printf("mysql_store_result():%s\n",mysql_error(&mysql));
		return -1;
	}
	rows = mysql_num_rows(res);
	printf("the total rows is :%d\n",rows);
	fields = mysql_num_fields(res);
	printf("the total fieldsis:%d\n",fields);
	while((row = mysql_fetch_row(res))) {
		for(i = 0;i < fields;i++) {
			printf("%s\t",row[i]);
		}
		printf("\n");
	}

	//插入删除示例
	query_str = "insert into info values(3,'laojibai','123456','0')";
	//执行插入请求
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(0 != rc) {
		printf("1mysql_real_query(): %s\n",mysql_error(&mysql));
		return -1;
	}
	//执行删除请求
	query_str = "delete from info where id=4";
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(0 != rc) {
		printf("2mysql_real_query(): %s\n",mysql_error(&mysql));
		return -1;
	}

	//查询插入与删除后的数据
	query_str = "select * from info";
	rc = mysql_real_query(&mysql,query_str,strlen(query_str));
	if(0 != rc) {
		printf("3mysql_real_query(): %s\n",mysql_error(&mysql));
		return -1;
	}
	res = mysql_store_result(&mysql);
	if(NULL == res) {
		printf("mysql_store_result():%s\n",mysql_error(&mysql));
		return -1;
	}
	rows = mysql_num_rows(res);
	printf("the total rows is :%d\n",rows);
	fields = mysql_num_fields(res);
	printf("the total fieldsis:%d\n",fields);
	while((row = mysql_fetch_row(res))) {
		for(i = 0;i < fields;i++) {
			printf("%s\t",row[i]);
		}
		printf("\n");
	}
	

	mysql_close(&mysql);
	return 0;
}
