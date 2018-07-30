/*2018-7-24 周二 */
/*fcntl_Lock.c 文件记录锁的应用 */
#include<stdio.h>
#include<unistd.h>		//read,write
#include<fcntl.h>		//fcntl open
#include<sys/types.h>	//lseek	open
#include<sys/stat.h>	//open
#include<string.h>		//memset

//释放或者设置锁，通过lock->l_type 属性
int lock_set(int fd,struct flock *lock)
{
	if(fcntl(fd,F_SETLK,lock) == 0)	{ //if :success  return 0  else return -1
		if(lock->l_type == F_RDLCK) {
					printf("set read lock,pid:%d\n",getpid());
					return 0;
		}
		else if(lock->l_type == F_WRLCK) {
			printf("set write lock,pid:%d\n",getpid());
		}
		else if(lock->l_type == F_UNLCK) {
			printf("release lock,pid:%d\n",getpid());
		}
	}
	else {
		perror("lock operation fail\n");
		return -1;
	}
	return 0;
}

//测试锁，只有当测试发现参数lock 指定的锁能被设置时，返回0
int lock_test(int fd,struct flock*lock)
{
	if(fcntl(fd,F_GETLK,lock) == 0)	{//success
		if(lock->l_type == F_UNLCK) {	//能按参数lock要求设置锁
			printf("look can be set in fd\n");
			return 0;
		}
	else {	//有不兼容的锁的存在，打印出设置该锁的进程ID
		if(lock->l_type == F_RDLCK) {
			printf("can't set lock,read lock has been set by:%d\n",lock->l_pid);
		}
		else if(lock->l_type == F_WRLCK) {
			printf("can't set lock,write lock has been set by:%d\n",lock->l_pid);
		}
		return -2;
		}
	}
	else { //失败，返回-1
		perror("get incompatible locks fail");
		return -1;
	}
}

int main(void)
{
	int fd;
	int ret;
	struct flock lock;
	char read_buf[32];
	
	//打开或创建文件
	if((fd = open("par.txt",O_CREAT | O_TRUNC | O_RDWR,S_IRWXU)) == -1)
		printf("open file is error");
	if(write(fd,"wang heng",10) != 10)
		printf("write is error");
	
	//初始化lock结构
	memset(&lock,0,sizeof(struct flock));
	lock.l_start = SEEK_SET;
	lock.l_whence = 0;
	lock.l_len = 0;

	//设置读锁
	lock.l_type = F_RDLCK;
	if(lock_test(fd,&lock) == 0) {	//测试可以设置锁
		lock.l_type = F_RDLCK;
		lock_set(fd,&lock);
	}

	//读数据
	lseek(fd,0,SEEK_SET);
	if(ret = read(fd,read_buf,10) < 0)
		printf("read the data is error");
	read_buf[ret] = '\0';
	printf("%s\n",read_buf);

	getchar();

	//设置写锁
	lock.l_type = F_WRLCK;
	if(lock_test(fd,&lock) == 0) {
		lock.l_type = F_WRLCK;
		lock_set(fd,&lock);
	}

	//释放锁
	lock.l_type = F_UNLCK;
	lock_set(fd,&lock);
	close(fd);
	return 0;
}





