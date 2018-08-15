#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<pthread.h>

int fd;
pthread_mutex_t mutex;
void my_err(char *string, int line)
{
	fprintf(stderr, "line:%d\n", line);
	perror(string);
	exit(0);
}

void thread1(void)
{	
	int len = 0;
	char rbuf[101] = {0};
	char wbuf[15] = "hello, word!";
	pthread_mutex_lock(&mutex);
	while(read(fd, rbuf, 1))
		len++;
	printf("len = %d\n", len);
	if(len > 100 - strlen(wbuf) +1)
	{
		printf("文件已满，请读取后在进行写入\n");
		pthread_mutex_unlock(&mutex);
	}
	else if(write(fd, wbuf, strlen(wbuf) + 1) < 0)
		my_err("write", __LINE__);
	else
		pthread_mutex_unlock(&mutex);
}

void thread2(void)
{	
	pthread_t tid1;
	int status;
	char rbuf[14];
	char temp[101];
	char str[101];
	int  i = 0, count = 0;
	pthread_mutex_lock(&mutex);
	if(lseek(fd, 0, SEEK_SET) < 0)
		my_err("lseek", __LINE__);
	while(read(fd, temp, 1))
	{		
		if(count < 10)
		{
			rbuf[count] = temp[0];
			count++;
			rbuf[count] = '\0';
		}
		else
		{	
			str[i] = temp[0];
			i++;
		}
		
	}
	if(count != 0)
	{
		printf("count = %d\n", count);
		printf("%s\n", rbuf);
		if(remove("exam") == -1)
			my_err("remove", __LINE__);
		fd = open("exam", O_RDWR | O_CREAT, 0644);
		write(fd, str, strlen(str) + 1);
	}
	pthread_mutex_unlock(&mutex);
}

int main()
{	
	pthread_t tid1, tid2;
	int status;
	pthread_mutex_init(&mutex, NULL);
	if( (fd = open("exam", O_RDWR | O_CREAT, 0644)) == -1)
		my_err("open", __LINE__);
	while(1)
	{
		pthread_create(&tid1, NULL, (void *)thread1, NULL);
		pthread_create(&tid2, NULL, (void *)thread2, NULL);
		getchar();
	}
	pthread_join(tid1, (void *)&status);
	pthread_exit((void *)&status);
}
