#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>

/*自定义的错误处理函数*/
void my_err(char *err_string, int line)
{
	fprintf(stderr,"line:%d   ",line);
	perror("err_string");
	exit(1);
}

/*锁的设置释放函数*/
int lock_set(int fd, struct flock * lock)
{
	if(fcntl(fd, F_SETLK, lock)== 0) //执行成功
	{
		if(lock->l_type == F_RDLCK)
			printf("set read lock, pid: %d\n",getpid());
		else if(lock->l_type == F_WELCK)
			printf("set write lock, pid: %d\n",getpid());
		else if(lock->l_type == F_UNLCK)
			printf("release lock, pid: %d\n",getpif());
	}
	else  //执行失败返回-1
	{
		perror("lock operation fail!\n");
		return -1;	
	}
}

/*测试锁，只有当测试发现参数lock指定的锁能被设置时，返回0*/
int lock_test(int fd, struct flock *lock)
{
	if(fcntl(fd,F_GETLK,lock)== 0) //执行成功
	{
		if(lock->l_type == F_UNLCK) //测试发现能按参数lock要求设置锁
		{
			printf("lock can be set in fd\n");
			return 0;
		}
		else //有不兼容的锁存在，打印出该锁的进程ID
		{
			if(lock->l_type == F_RDLCK)
				printf("cant't set lock,read lock has been set by: %d\n",lock->pid);
			else if(lock->l_type == F_WELCK)
				printf("can't set lock,write lock has been set by: %d\n",lock->pid);
			return -2;

		}	
	}
	else //执行失败
	{
		perror("get incompatible locks fail");
		return -1;
	}

}

int main()
{
	int fd;
	int ret;
	strcut flock lock;
	char read_buf[32];

	/*打开或创建文件*/
	if((fd = open("example_4",O_CREAT | O_TRUNC | O_RDWR, S_IRWXU)) ==-1 )
	{
		my_err("open"__LINE__);
	}

	/*初始化lock结构*/
	memset(&lock ,0, sizeof(struct flock));
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;

	/*设置读锁*/
	lock.l_type = F_RDLCK;
	if(lock_test(fd, &lock)==0) //测试可以设置锁
	{
		lock.l_type = F_RDLCK;
		lock_set(fd,&lock);
	}
}

