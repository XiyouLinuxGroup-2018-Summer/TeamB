/*************************************************************************
	> File Name: shell.c
	> Author: 
	> Mail: 
	> Created Time: 2018年08月05日 星期日 20时10分35秒
 ************************************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<dirent.h>
char command[256];
int explan();//解析本命令
int find();//去内核里面查找有没有该文件
/*flag=1:有'|'管道命令
flag=2:有'<'输入重定向命令
flag=3:有'>'标准输出重定向“覆盖”命令
flag=4:有">>"标准输出重定向“追加”命令
flag=5:有'&'后台运行命令
flag=6:有"cd"内建命令*/
int flag=0;
int find(char *name)
{
    int i;
    struct dirent *dir_e;
    DIR *dir;//打开目录返回的目录流
    char *path[]={"/usr/local/sbin","/usr/local/bin","/usr/sbin","/usr/bin","/sbin","/bin","/usr/games","/usr/local/games","/snap/bin"};
    int t=9;//说明有9个路径
    for(i=0;i<9;i++)
    {
        if((dir=opendir(path[i]))==NULL)
            printf("open fail\n");
        else
        {
            while((dir_e=readdir(dir))!=NULL)
            {
                if(strcmp(name,dir_e->d_name)==0)
                {
                    closedir(dir);
                    return 1;
                }
            }
        }
        closedir(dir);
    }
    return 0;
}
void explain()
{
    char *rear[256];
    int N;//接收传回来的值判断
    int fd,fd2;
    pid_t pid;
    int flag=0;
    char name_pre[20][20];//定义特殊符号前面的命令
    char name_rear[20][20];//定义了特殊符号后面的命令
    char file[30];//定义一个文件名
    int t=0;//定义一个变量用来记录当前
    int i=0;
    int j=0;
    int k=0;
    char demand[20][256];//提取命令
    char *arg[256];
    //提取命令
    //memset(rear,'\0',sizeof(char *)*256);
    //memset(name_rear,'\0',sizeof(char)*20*20);
    //memset(file,'\0',sizeof(char)*30);
    //memset(demand,'\0',sizeof(demand)*20*256);
    //memset(arg,'\0',sizeof(char *)*256);
    //memset(name_pre,'\0',sizeof(char)*20*20);
    int len=0;//记录有多少个
    while(command[len]!='\n')
    {
        if(command[len]==' ')
        {
            len++;
            i++;
            t=i;
            j=0;
        }
        else
        {
            demand[i][j]=command[len];
            len++;
            j++;
        }
    }
    for(i=0;i<=t;i++)
    {
        arg[i]=(char *)demand[i];
    }
    arg[i]=NULL;
    for(i=0;i<=t;i++)
    {
        if((strlen(demand[i])==1)&&(demand[i][0]=='|'))
        {
            arg[i]=NULL;
            //把管道之前的命令提取出来
            for(k=0;k<i;k++)
            {
                strcpy(name_pre[k],demand[k]);
            }
            //把管道之后的命令提取出来
            j=0;
            for(k=i+1;k<=t;k++)
            {
                strcpy(name_rear[j],demand[k]);
                j++;
            }
            for(k=0;k<j;k++)
            {
                rear[k]=(char *)name_rear[k];
            }
            rear[j]=NULL;
            flag=1;
            break;
        }
        else if((demand[i][0]=='<')&&(strlen(demand[i])==1))
        {
            strcpy(file,demand[i+1]);
            j=0;
            arg[i]=NULL;
            //把输入重定向之前的命令提取出来
            for(k=0;k<i;k++)
            {
                strcpy(name_pre[j],demand[k]);
                j+=1;
            }
            flag=2;
            break;
        }
        else if((demand[i][0]=='>')&&(strlen(demand[i])==1))
        {
            arg[i]=NULL;
            strcpy(file,demand[i+1]);
            j=0;
            //把输出覆盖重定向之前的命令提取出来
            for(k=0;k<i;k++)
            {
                strcpy(name_pre[j],demand[k]);
                j+=1;
            }
            flag=3;
            break;
        }
        else if(strcmp(demand[i],">>")==0)
        {
            arg[i]=NULL;
            strcpy(file,demand[i+1]);
            j=0;
            //把输出追加重定向之前的命令提取出来
            for(k=0;k<i;k++)
            {
                strcpy(name_pre[j],demand[k]);
                j++;
            }
            flag=4;
            break;
        }
        else if((demand[i][0]=='&')&&(strlen(demand[i])==1))
        {
            flag=5;
            break;
        }
    }
     if((arg[0]!=NULL) &&(strcmp(arg[0],"cd") == 0) )
         {
             if(arg[1] == NULL)
             {
                 return ;
             }
             if(strcmp(arg[1],"~") == 0)
             {
                 //~替换为家目录
                 strcpy(arg[1],"/home/k/");
             }
             if(chdir(arg[1]) == -1)
             {
                 perror("cd");
             }
             return;
         }
   /* if(strcmp(arg[0],"cd")==0)//执行cd，在主进程里
    {
        chdir(arg[1]);
        return ;
    }*/
    if((pid=fork())<0)
    {
        printf("fork error\n");
        exit(-1);
    }
    switch(flag)
    {
        case 0://一般命令
            {
                if(pid==0)
                {
                    N=find(demand[0]);
                    if(N!=1)
                        exit(0);
                    execvp(demand[0],arg);
                    exit(0);
                }
                break;
            }
        case 1://管道
            {
                if(pid==0)
                {
                    pid_t pid2;
                    int fd2;
                    if((pid2=fork())<0)
                    {
                        printf("fork2 fail\n");
                        exit(-1);
                    }
                    if(pid2==0)
                    {
                        N=find(name_pre[0]);
                        if(N!=1)
                            exit(0);
                        fd2=open("haha",O_WRONLY|O_CREAT|O_TRUNC,0644);
                        dup2(fd2,1);
                        execvp(name_pre[0],arg);
                        exit(0);
                    }
                    waitpid(pid2,NULL,0);
                    N=find(rear[0]);
                    if(N!=1)
                    {
                        exit(0);
                    }
                    fd2=open("haha",O_RDONLY);
                    dup2(fd2,0);
                    execvp(rear[0],rear);
                    exit(0);
                }
                break;
            }
        case 2://输入重定向命令
            {
                if(pid==0)
                {
                    N=find(name_pre[0]);
                    if(N!=1)
                        exit(0);
                    fd=open(file,O_RDWR|O_CREAT|O_TRUNC,0644);
                    dup2(fd,0);
                    execvp(name_pre[0],arg);
                    exit(0);
                }
                break;
            }
        case 3://输出覆盖重定向
            {
                if(pid==0)
                {
                    N=find(name_pre[0]);
                    if(N!=1)
                        exit(0);
                    fd=open(file,O_RDWR|O_CREAT|O_TRUNC,0644);
                    dup2(fd,1);
                    execvp(name_pre[0],arg);
                    exit(0);
                }
                break;
            }
        case 4://输出追加重定向
            {
                if(pid==0)
                {
                    N=find(name_pre[0]);
                    if(N!=1)
                        exit(0);
                    fd=open(file,O_RDWR|O_CREAT|O_APPEND,0644);
                    dup2(fd,1);
                    execvp(name_pre[0],arg);
                    exit(0);
                }
                break;
            }
        case 5://后台运行命令
            {
                return;
            }
        default:break;
    }
    waitpid(pid,NULL,0);
}
void getinput()
{
    int len=0;
    char ch;
    while((len<256)&&(ch!='\n'))
    {
        ch=getchar();
        command[len++]=ch;
    }
    if(len==256)
    {
        printf("command is too long\n");
        exit(-1);
    }
    command[len]='\0';
}
int main(void)
{
    int i,k=0;
    while(1)
    {
        memset(command,0,256);
        char pwd[128];
    getcwd(pwd,sizeof(pwd)-1);
    int len = strlen(pwd);
    char*p = pwd+len-1;
    while(*p != '/' && len--)
    {
        p--;
    }
    p++;
    printf(" ~/");
    printf("%s]@",p);
        printf("lalashell$:");
        getinput();
        /*判断命令是不是退出的命令*/
        if(strcmp(command,"exit\n")==0)
        {
            printf("ha\n");
            exit(0);
        }
        explain();
    }
}

