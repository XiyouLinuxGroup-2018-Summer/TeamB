/*************************************************************************
	> File Name: ls.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月30日 星期一 01时12分27秒
 ************************************************************************/
/*定义的头文件*/
#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<limits.h>
#include<dirent.h>
#include<grp.h>
#include<pwd.h>
#include<errno.h>
#include<fcntl.h>
//定义一个结构体来存储读取到目录下面的文件名字，方便排序
struct name
{
    char Name[100];
    struct name *next;
};
void my_err();//自定的错误输出函数
struct name *get_file();//冒泡法获取文件名或者目录
void out();//输出信息
void out_l();//输出属性
void put_file_R();//关于-R的
/*自定义的错误处理函数*/
void my_err(const char *err_string,int err_line)
{
    fprintf(stderr,"line:%d ",err_line);
    perror(err_string);
    exit(1);
}
/*关于-R的*/
void put_file_R(struct name *phead,char path[100000],int M)
{
    int flag=0;
    char path2[100000];
    DIR *dir,*dirr;//判断是不是目录
    struct dirent *dir_e,*dir_e2;//读目录，主要用于读目录里面的名字
    struct name *p;//定义是phead的下一个
    struct name *ph,*pnew;
    strcpy(path2,path);
    ph=(struct name *)malloc(sizeof(struct name));
    ph->next=NULL;
    p=phead->next;
    while(p!=NULL)
    {
        if(strcmp(p->Name,"..")==0)
            ;
        else if((strlen(p->Name)==1)&&(p->Name[0]=='.'))
            ;
        else
        {
            chdir(path);
            if((dir=opendir(p->Name))==NULL)//说明是一个文件
            {
                if(M==2)
                    printf("%s\n",p->Name);
            }
            else//说明是一个目录
            {
                if(M==2)
                    printf("%s是一个目录\n",p->Name);
                path[strlen(path)]='/';
                strcat(path,p->Name);
                chdir(path);
                while((dir_e=readdir(dir))!=NULL)
                {
                    if(strcmp(dir_e->d_name,"..")==0)
                        ;
                    else if((strlen(dir_e->d_name)==1)&&(dir_e->d_name[0]=='.'))
                        ;
                    else
                    {
                        while(flag!=1)
                        {
                            if((dirr=opendir(dir_e->d_name))==NULL)//说明是一个文件
                            {
                                if(M==2)
                                    printf("%s\n",dir_e->d_name);
                                flag=1;
                            }
                            else//说明是一个目录
                            {
                                path[strlen(path)]='/';
                                strcat(path,dir_e->d_name);
                                chdir(path);
                                while((dir_e2=readdir(dirr))!=NULL)
                                {
                                    if(strcmp(dir_e2->d_name,"..")==0)
                                        ;
                                    else if((strlen(dir_e2->d_name)==1)&&(dir_e2->d_name[0]=='.'))
                                        ;
                                    else
                                    {
                                        pnew=(struct name *)malloc(sizeof(struct name));
                                        strcpy(pnew->Name,dir_e2->d_name);
                                        pnew->next=ph->next;
                                        ph->next=pnew;
                                    }
                                }
                                if(M==1)
                                {
                                    out_l(ph);
                                    put_file_R(ph,path,1);
                                }
                                else
                                {
                                    put_file_R(ph,path,2);
                                }
                                ph->next=NULL;
                                memset(path,'\0',strlen(path));
                                strcpy(path,"/home/lala");
                                path[strlen(path)]='/';
                                strcat(path,p->Name);
                            }
                        }
                        flag=0;
                    }
                }
            }
        }
        memset(path,'\0',strlen(path));//每次清空
        strcpy(path,path2);
        p=p->next;
    }
}
/*冒泡法排序获取目录下各个文件和目录的名称*/
struct name *get_file(char *path,int flag)
{
    int i;
    int num=0;//使用冒泡排序就要看有多少个，然后循环要用
    struct name *phead,*pnew,*p,*pn;//定义链表
    //链表初始化
    phead=(struct name *)malloc(sizeof(struct name));
    phead->next=NULL;
    DIR *dir;//目录打开返回的目录标识符
    struct dirent *dir_e;//打开目录后p=phead->next;
    //打开目录，错误返回出错信息
    if((dir=opendir(path))==NULL)
        my_err("opendir",__LINE__);
    //读取目录中的文件名，然后存储到列表中去
    while((dir_e=readdir(dir))!=NULL)
    {
        if(flag==1)//不读隐藏文件
        {
            if(dir_e->d_name[0]!='.')
            {
                pnew=(struct name *)malloc(sizeof(struct name));
                strcpy(pnew->Name,dir_e->d_name);
                pnew->next=phead->next;
                phead->next=pnew;
                num+=1;
            }
        }
        else//全部文件都读出来
        {
            pnew=(struct name *)malloc(sizeof(struct name));
            strcpy(pnew->Name,dir_e->d_name);
            pnew->next=phead->next;
            phead->next=pnew;
            num+=1;
        }
    }
    //冒泡排序
    while(num--)
    {
        p=phead;
        pnew=phead->next;
        pn=pnew->next;
        while(pn!=NULL)
        {
            if(strcasecmp(pnew->Name,pn->Name)>0)
            {
                pnew->next=pn->next;
                p->next=pn;
                pn->next=pnew;
                p=pn;
                pn=pnew->next;
            }
            else
            {
                p=pnew;
                pnew=pn;
                pn=pn->next;
            }
        }
    }
    return phead;
}
/*输出信息*/
void out(struct name *phead)
{
    int len=0;//记录有多少个
    struct name *p;
    p=phead->next;
    while(p!=NULL)
    {
        len+=1;
        printf("%-20s",p->Name);
        if(len%6==0)
            printf("\n");
        p=p->next;
    }
    printf("\n");
}
/*打印文件属性*/
void out_l(struct name *phead,char *path)
{
    struct passwd *user;
    struct group *grop;
    struct stat buf;
    struct name *p;
    chdir(path);
    p=phead->next;
    while(p!=NULL)
    {
        if(stat(p->Name,&buf)==-1)
            my_err("stat",__LINE__);
        else
        {
            /*打印文件属性*/
            if(S_ISLNK(buf.st_mode))
                printf("l");
            else if(S_ISREG(buf.st_mode))
                printf("-");
            else if(S_ISDIR(buf.st_mode))
                printf("d");
            else if(S_ISCHR(buf.st_mode))
                printf("c");
            else if(S_ISBLK(buf.st_mode))
                printf("b");
            else if(S_ISFIFO(buf.st_mode))
                printf("p");
            else if(S_ISSOCK(buf.st_mode))
                printf("s");
            /*获取并打印文件所有者的权限*/
            if(buf.st_mode&S_IRUSR)
                printf("r");
            else
                printf("-");
            if(buf.st_mode&S_IWUSR)
                printf("w");
            else
                printf("-");
            if(buf.st_mode&S_IXUSR)
                printf("x");
            else
                printf("-");
            /*获取并打印组权限*/
            if(buf.st_mode&S_IRGRP)
                printf("r");
            else
                printf("-");
            if(buf.st_mode&S_IWGRP)
                printf("w");
            else
                printf("-");
            if(buf.st_mode&S_IXGRP)
                printf("x");
            else
                printf("-");
            /*获取并打印其他人的权限*/
            if(buf.st_mode&S_IROTH)
                printf("r");
            else
                printf("-");
            if(buf.st_mode&S_IWOTH)
                printf("w");
            else
                printf("-");
            if(buf.st_mode&S_IXOTH)
                printf("x");
            else
                printf("-");
            /*打印文件的连接数*/
            printf("  %-2ld  ",buf.st_nlink);
            /*获取并打印文件的所有者*/
            user=getpwuid(buf.st_uid);
            printf("%-8s",user->pw_name);//输出8位，不足的右边用空格补齐
            /*获取并打印文件的用户组*/
            grop=getgrgid(buf.st_gid);
            printf("%-8s",grop->gr_name);//输出8位，不足的右边用空格补齐
            /*打印文件的容量*/
            printf("%6ld",buf.st_size);
            /*打印修改日期*/
            char time[30];
            strcpy(time,ctime(&buf.st_mtime));
            time[strlen(time)-1]='\0';
            printf(" %s",time);
            /*打印文件名*/
            printf("  %s\n",p->Name);
            p=p->next;
        }
    }
}
/*主函数，输入命令和参数或者文件*/
int main(int argc,char **argv)
{
    char program[3];//存取-al 之类的
    char program_[4];//存取-alR
    struct name *phead;//定义了结构体的头,存return返回的
    int i;
    char directory_work[PATH_MAX+1];//定义了当前目录
    switch(argc)
    {
        case 1://说明只有./a.out 要输入的东西不够
                {
                    printf("%s <mode num> <target file>\n",argv[0]);
                    exit(0);
                    break;
                }
        case 2:
                {
                    if(strcmp(argv[1],"ls")!=0)
                    {
                        printf("%s: 未找到命令\n",argv[1]);
                        exit(0);
                    }
                    else//单纯只有ls 没有后面的参数
                    {
                        if(getcwd(directory_work,512)==NULL)
                            my_err("getcwd",__LINE__);
                        else
                        {
                            phead=get_file(directory_work,1);//调用一个函数直接输出
                            out(phead);//输出信息
                        }
                    break;
                }
        case 3:
                {
                    if(argv[2][0]=='-')//说明有参数 a/l/R,并且没有指定要查询的目录
                    {
                        if(strlen(argv[2])==2)//只有一个参数
                        {
                            switch(argv[2][1])
                            {
                                case 'a':
                                    {
                                        if(getcwd(directory_work,512)==NULL)
                                            my_err("getcwd",__LINE__);
                                        else
                                        {
                                            phead=get_file(directory_work,2);
                                            out(phead);
                                        }
                                        break;
                                    }
                                case 'l':
                                    {
                                        if(getcwd(directory_work,512)==NULL)
                                            my_err("getcwd",__LINE__);
                                        else
                                        {
                                            phead=get_file(directory_work,1);
                                            out_l(phead,directory_work);
                                        }
                                        break;
                                    }
                                case 'R':
                                    {
                                        if(getcwd(directory_work,512)==NULL)
                                            my_err("getcwd",__LINE__);
                                        else
                                        {
                                            phead=get_file(directory_work,2);
                                            put_file_R(phead,directory_work,2);
                                        }
                                        break;
                                    }
                                default:
                                    {
                                        printf("ls: 不适用的选项--%c\n",argv[2][1]);
                                        break;
                                    }
                            }
                        }
                        else if(strlen(argv[2])==3)//有两种参数的组合al aR lR
                        {
                            strcpy(program,argv[2]);
                            if(getcwd(directory_work,512)==NULL)
                                my_err("getcwd",__LINE__);
                            else
                            {
                                if((strcmp(program,"-al")==0)||(strcmp(program,"-la")==0))
                                {
                                    phead=get_file(directory_work,2);
                                    out_l(phead,directory_work);
                                }
                                else if((strcmp(program,"-aR")==0)||(strcmp(program,"-Ra")==0))
                                {
                                    phead=get_file(directory_work,2);
                                    put_file_R(phead,directory_work,2);
                                }
                                else if((strcmp(program,"-Rl")==0)||(strcmp(program,"-lR")==0))
                                {
                                    phead=get_file(directory_work,2);
                                    out_l(phead,directory_work);
                                    put_file_R(phead,directory_work,1);
                                }
                            }
                        }
                        else if(strlen(argv[2])==4)//有三种参数的组合alR
                        {
                            strcpy(program_,argv[2]);
                            if(getcwd(directory_work,512)==NULL)
                                my_err("getcwd",__LINE__);
                            else
                            {
                                if((strcmp(program_,"-alR")==0)||(strcmp(program_,"-aRl")==0)||(strcmp(program_,"-laR"))||(strcmp(program_,"-lRa")==0)||(strcmp(program_,"-Rla")==0)||(strcmp(program_,"-Ral")==0))
                                {
                                    phead=get_file(directory_work,2);
                                    put_file_R(phead,directory_work,1);
                                }
                            }
                        }
                    }
                    else//说明只有目录，无参数
                    {
                        strcpy(directory_work,argv[2]);
                        phead=get_file(directory_work,1);
                        out(phead);
                    }
                    break;
                }
        case 4:
                {
                    if(strlen(argv[2])==2)//说明有指定的目录，也有参数
                    {//说明在指定目录下只有一个参数
                        switch(argv[2][1])
                            {
                                case 'a':
                                    {
                                        strcpy(directory_work,argv[3]);
                                        phead=get_file(directory_work,2);
                                        out(phead);
                                        break;
                                    }
                                case 'l':
                                    {
                                        strcpy(directory_work,argv[3]);
                                        phead=get_file(directory_work,1);
                                        out_l(phead,directory_work);
                                        break;
                                    }
                                case 'R':
                                    {
                                        strcpy(directory_work,argv[3]);
                                        phead=get_file(directory_work,2);
                                        put_file_R(phead,directory_work,2);
                                        break;
                                    }
                                default:
                                    {
                                        printf("ls: 不适用的选项--%c\n",argv[2][1]);
                                        break;
                                    }
                             }
                    }
                    else//说明有组合参数
                    {
                        if(strlen(argv[2])==3)//有两种参数的组合al aR lR
                        {
                            strcpy(program,argv[2]);
                            strcpy(directory_work,argv[3]);
                            if((strcmp(program,"-al")==0)||(strcmp(program,"-la")==0))
                            {
                                phead=get_file(directory_work,1);
                                out_l(phead,directory_work);
                            }
                            else if((strcmp(program,"-aR")==0)||(strcmp(program,"-Ra")==0))
                            {
                                phead=get_file(directory_work,2);
                                put_file_R(phead,directory_work,2);
                            }
                            else if((strcmp(program,"-Rl")==0)||(strcmp(program,"-lR")==0))
                            {
                                phead=get_file(directory_work,2);
                                put_file_R(phead,directory_work,1);
                            }
                        }
                        else if(strlen(argv[2])==3)//有两种参数的组合alR
                        {
                            strcpy(program_,argv[2]);
                            strcpy(directory_work,argv[3]);
                            if((strcmp(program_,"-alR")==0)||(strcmp(program_,"-aRl")==0)||(strcmp(program_,"-laR"))||(strcmp(program_,"-lRa")==0)||(strcmp(program_,"-Rla")==0)||(strcmp(program_,"-Ral")==0))
                            {
                               phead=get_file(directory_work,2);
                                put_file_R(phead,directory_work,1);
                            }
                        }
                    }
                    break;
                }
        default://输入有问题，没有输入ls命令
                {
                    printf("%s <mode num> <target file>\n",argv[0]);
                    exit(0);
                }
        }
    }
}

