#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
#include<ctype.h>
#include<pwd.h> //用于打印用户名
#define BUFFSIZE 64

#define normal 0        //一般的命令
#define out_redirect 1  //输出重定向
#define in_redirect 2   //输入重定向
#define have_pipe 3     //命令中有管道

void GetLoginName();
void GetHostName();
void GetDir();              //打印提示符
void get_input(char *);     //得到输入的命令
void explan_input(char*, int *, char a[][8]);//对于输入的命令进行解析
//此地注意,二维数组作为函数参数,第一维长度可以不确定,但是必须指定第二维长度,char a[][]

void do_cmd(int, char a[][8]);  //执行命令
int find_command(char *);       //查找命令中的可执行程序


int main(int argc, char **argv)
{
    char buf[BUFFSIZE];
    char arglist[10][8];
    int argcount;

    while(1)
    {
        memset(buf,0,BUFFSIZE);
        GetLoginName();
        GetHostName();
        GetDir();       //打印提示符
        fflush(stdout);

        get_input(buf); //读取用户输入
        if(strcmp(buf,"exit")==0 || strcmp(buf,"logout")==0)
            break;
        int i;
        for(i=0; i<10; i++) //初始化存放命令和参数的组
        {
            arglist[i][0] = '\0';
        }
        argcount = 0;                           //计数,命令中词汇量
        explan_input(buf,&argcount,arglist);    //解析命令
//       do_cmd(argcount,arglist);               //执行命令
    }
    return 0;
}
    

void GetLoginName()
{
    struct passwd* pass;
    pass = getpwuid(getuid());
    printf("[%s@",pass->pw_name);
}
void GetHostName()
{
    char name[128];
    gethostname(name,sizeof(name)-1);
    printf("%s",name);
}
void GetDir()
{
    char pwd[128];
    getcwd(pwd,sizeof(pwd)-1);
    int len = strlen(pwd);
    char*p = pwd+len-1;
    while(*p != '/' && len--)
    {
        p--;
    }
    p++;
    printf("%s]@",p);
}

void get_input(char *buf)
{
    fgets(buf,BUFFSIZE,stdin);
    int len; 
    len = sizeof(buf);
    if(len >= BUFFSIZE)
    {
        printf("ERROR: command is too long!\n");
        exit(-1);
    } 
    buf[len -1] = '\0'; //去除换行符

}

/*解析buf中的命令存入arglist中,命令及参数个数为argcount
ls -l arglist[0]=ls, arglist[1]=-l*/
/*void explan_input(char *buf, int *argcount,char arglist[][8])
{
    char *p = buf;
    ///将用户输入的整行字符拆分成一个个单词,存入二维数组的每一行中


}*/