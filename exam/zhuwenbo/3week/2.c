#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<pthread.h>


void my_err(char *string, int line)
{
	fprintf(stderr, "line:%d\n", line);
	perror(string);
	exit(0);
}

void thread1(void)
{	
	int fd;
	int s;
	if( (fd = open("exam", O_WRONLY | O_CREAT, 0644)) == -1)
		my_err("open", __LINE__);
	char wbuf[15] = "hello, word!";
	if(write(fd, wbuf, strlen(wbuf) + 1) < 0)
		my_err("write", __LINE__);
}

void thread2(void)
{	
	pthread_t tid1;
	int status;
	char rbuf[101];
	char temp[101];
	int len;
	int i = 0;
	int fd;
	int count;
	while(1)
	{
		count = 0;
		pthread_create(&tid1, NULL, (void *)thread1, NULL);
		pthread_join(tid1, (void *)&status);
		if( (fd = open("exam", O_RDONLY)) == -1)
			my_err("open", __LINE__);
		while(1)
		{
			if(read(fd, temp, 1) == 0)
			{
				
				if(count != 0)
				{	
					printf("%s\n", rbuf);
					break;
				}
				else
				{
					printf("请向文件中写入数据\n");
					break;
				}
			}
			count++;
			rbuf[i] = temp[0];
			i++;
			rbuf[i] = '\0';
		}
		if(count != 0)
		{
			if(remove("exam") == -1)
				my_err("remove", __LINE__);
		}
		getchar();
	}
}

int main()
{	
	pthread_t tid2;
	int status;
	pthread_create(&tid2, NULL, (void *)thread2, NULL);
	pthread_join(tid2, (void *)&status);
	pthread_exit((void *)&status);
}
