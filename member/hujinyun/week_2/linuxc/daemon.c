#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<signal.h>
#include<sys/param.h>
#include<syslog.h>
#include<string.h>
/*
int init_daemon(void)
{
	int i;
	pid_t pid;
	pid = fork();
	if(pid>0)
		exit(0);
	if(pid<0)
	//	return -1;//exit(-1);
		exit(-1);
	setsid();
	pid = fork();
	if(pid > 0)
		exit(0);
	if(pid <0)
		exit(-1);

	for(i=0; i<NOFILE; close(i++));

	chdir("/");

	umask(0);

	signal(SIGCHLD,SIG_IGN);

	return 0;

}

int main()
{
	time_t now;
	init_daemon();
	syslog(LOG_USER|LOG_INFO,"测试守护进程\n");
	while(1)
	{
		sleep(8);
		time(&now);
		syslog(LOG_USER|LOG_INFO,"系统时间:\t%s\t\t\n",ctime(&now));
	
	}

}*/


//#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>

// 守护进程初始化函数
void init_daemon()
{
    pid_t pid;
    int i = 0;

    if ((pid = fork()) == -1) {
        printf("Fork error !\n");
        exit(1);
    }
    if (pid != 0) {
        exit(0);        // 父进程退出
    }

    setsid();           // 子进程开启新会话，并成为会话首进程和组长进程
    chdir("/tmp");      // 改变工作目录
    umask(0);           // 重设文件掩码
    for (; i < getdtablesize(); ++i) {
        close(i);        // 关闭打开的文件描述符
    }

    return;
}

int main(int argc, char *argv[])
{
    int fp;
    time_t t;
    char buf[] = {"This is a daemon:  "};
    char *datetime;
    int len = 0;

    // 初始化 Daemon 进程
    init_daemon();
    // 每隔一分钟记录运行状态
    while (1) {
        if (-1 == (fp = open("/tmp/daemon.log", O_CREAT|O_WRONLY|O_APPEND, 0600))) {
            printf("Open file error !\n");
            exit(1);
        }
        len = strlen(buf);
        write(fp, buf, len);
        t = time(0);
        datetime = asctime(localtime(&t));
        len = strlen(datetime);
        write(fp, datetime, len);
        close(fp);
        sleep(60);
    }
    return 0;
}
