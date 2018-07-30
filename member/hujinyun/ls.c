
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<unistd.h>
#include<linux/limits.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<errno.h>
#include<malloc.h>
#define MAXROWLEN 20 //一行显示的最多字符数

int g_leave_len = MAXROWLEN; //一行剩余长度，用于对齐
int g_maxlen = 0; //存放某目录下最长文件名的长度

 void display_dir(DIR *ret_opendir);

void my_err(char*string, int line)
{
    fprintf(stderr,"line: %d ",line);
    perror(string);
    exit(1);
}

void list_dir(const char *pathname)
{
    char nextpath[PATH_MAX];

    DIR * ret_opendir = opendir(pathname); //打开目录“pathname“
    if(ret_opendir==NULL)
    {
        my_err("opendir",__LINE__);
    }

    printf("%s:\n",pathname);//显示pathname目录路径
    display_dir(ret_opendir);//显示pathname目录下所有非隐藏文件名称

    struct dirent* ret_readdir = NULL; //定义readdir函数返回的结构体变量
    while(ret_readdir = readdir(ret_opendir))//判断是否读到目录尾
    {
        char *filename = ret_readdir->d_name;

        int end = 0;//优化显示路径（处理”./test/“与"./test"）
        while(pathname[end])
        end++;
        strcpy(nextpath,pathname);
        if(pathname[end-1] != '/')
            strcat(nextpath,"/");
        strcat(nextpath,filename);

        struct stat file_message = {};//定义stat函数返回的结构体变量
        int ret_stat = lstat(nextpath, &file_message); //获取文件信息
        if(ret_stat == -1) //stat读取文件错误则输出文件信息
            printf("%s error!",filename);
 //       if(flag == 1) 
        else if(S_ISDIR(file_message.st_mode) && filename[0] != '.')//筛除”.“、”..“与隐藏文件
        {
            list_dir(nextpath);
        }

    }
    closedir(ret_opendir);

}
    int display_len(char *name)
{
//    printf("6\n");
	int i,sum = 0;
	for(i=0;i<strlen(name); i++)
	{
		if(name[i]<0) //
			sum++;
	}
	sum/=3;
	return (strlen(name)-sum);
}
/*打印所有的文件名（只打印文件名）
 *此函数每次只打印一个
 *主要用于输出对齐
 * */
int display_single(char *name)//打印所有文件名（上下对齐）
{
	int i,len;
	//如果本行不足以打印一个文件名则换行
	len = display_len(name);
	if(len > g_leave_len)
	{
		printf("\n");
		g_leave_len = MAXROWLEN;
	}
	len = MAXROWLEN - len; //此时的len表示输出空格数
	printf("%-s",name);
	for(i=0;i<len; i++)
	{
		printf(" ");
	}
	printf("  ");
//    printf("2");
	g_leave_len -= (g_maxlen + 2);

}
    //打印所读取的文件
    void display_dir(DIR *ret_opendir)
    {
        struct dirent* ret_readdir = NULL; //定义readdir函数返回的结构体变量
        while((ret_readdir = readdir(ret_opendir))!=NULL) //判断是否读到文件末尾
        {
            char *filename = ret_readdir->d_name; //获取文件名
            if(filename[0]!='.') //不输出当前目录、上一级目录与隐藏目录
            display_single(ret_readdir->d_name );
//            printf("%s\t",ret_readdir->d_name);//打印文件名
        }
//rewinddir(ret_opendir);// 将文件流的指针拨回起始位置!重要
        puts(" ");
        puts(" ");
    }
/*    void display_dir_2(DIR *ret_opendir)
    {
        struct dirent* ret_readdir = NULL; //定义readdir函数返回的结构体变量
        while((ret_readdir = readdir(ret_opendir))!=NULL) //判断是否读到文件末尾  
         //当ptr到达内存中不可访问的区域（即argv[n2-1]的下面，函数栈帧的地址），就会引发SIGSEGV信号。
        {
            char *filename = ret_readdir->d_name; //获取文件名
            if(filename[0]!='.') //不输出当前目录、上一级目录与隐藏目录
            display_single(ret_readdir->d_name );
//            printf("%s\t",ret_readdir->d_name);//打印文件名
        }
//        rewinddir(ret_opendir);// 将文件流的指针拨回起始位置
 //   if(ret_readdir = )
        puts(" ");
        puts(" ");
    }
*/
int main(int argc, const char **argv)
{
    char path[PATH_MAX+1] = {};

    if(argc == 1)
    {
        strcpy(path,".");
            struct stat file_message = {};
            int ret_stat = lstat(path,&file_message);
            if(ret_stat == -1)
            {
                my_err("lstat",__LINE__);
            }
       
         if(S_ISDIR(file_message.st_mode))//判断是否为目录
             {
                    DIR * ret_opendir = opendir(path); //打开目录“path“
                    if(ret_opendir==NULL)
                    {
                       my_err("opendir",__LINE__);
                    }
                    display_dir(ret_opendir);//显示pathname目录下所有非隐藏文件名称
             }
     }
     printf("test");
   
    if( (argc == 2) && !(strcmp(argv[1],"-R")))//判断命令格式  //ls -R情况
    strcpy(path,".");
    else if(argc  > 3)
    {
        printf("格式错误");
        exit(EXIT_FAILURE);
    }
    else if(argc == 3)//指定路径情况 eg:ls -R /etc
        strcpy(path,argv[2]);

        if(!(strcmp(argv[1],"-R")))
        {
            struct stat file_message = {};
            int ret_stat = lstat(path,&file_message);
            if(ret_stat == -1)
            {
                my_err("lstat",__LINE__);
            }
            if(S_ISDIR(file_message.st_mode))//判断是否为目录
                list_dir(path);
            else
            printf("not a dir!\n");
        
        }
        else
        {
            printf("err in main!\n");
            exit(EXIT_FAILURE);
        }
}