/*
1. 分析代码并说明：
a. 最终输出多少次"hello, world?"；
b. 最终将产生共多少个进程。
int main(int argc, char *argv[])
{
for (int i = 0; i < 2; i++)
{
pid_t pid = fork();
printf("hello, world?");
}
 return 0;
}
*/
//首先第一次循环产生一个子进程和一个父进程，由于cpu算法调度的问题，先执行主进程，打印一次hello,word, 然后主进程没有退出，开始循环接着fork，当父进程循环执行完之后轮到子进程开始执行程序，总共打印8次hello，word。产生了四个进程.8个hello，word因为缓冲区的问题，既没有加\n清空缓冲区，主进程也没有exit退出清除缓冲区，导致子进程既成了父进程的缓冲区里的内容，所以打印八次。
