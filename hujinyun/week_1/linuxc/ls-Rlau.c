#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<grp.h>
#include<pwd.h>
#include<time.h>
#include<stdlib.h>

void display_do_stat();
void display_sort_R(int );
void display_ls(char [],int);
void display_count_byt(char *,char *,struct stat *);
void display_sortbytime();
void display_sort();
void display_stat_info(char *,char *,struct stat *);
void mode_detail(int ,char []);
char *gid_to_name(gid_t);
char *uid_to_name(uid_t);

void display_dir(DIR *ret_opendir);
int display_single(char *name);//打印所有文件名（上下对齐）
int display_len(char *name);
void list_dir( char *pathname);
    
#define MAXROWLEN 20 //一行显示的最多字符数

int g_leave_len = MAXROWLEN; //一行剩余长度，用于对齐
int g_maxlen = 0; //存放某目录下最长文件名的长度

void do_sort1(int );
int printf_color(char name[]);

char ftname[100][100];  //目录或文件名
char uname[100][100];
char command[100][100];
char diirectory[100][100];
int flagRl=0;
int f;  //同一目录下目录/文件个数（ftname[][]数组的变量个数）
int flag =0;
char  name[100]=" ";
int allbyt=0;
int t=0;

struct node{
	char name2[100];
	char name1[100];
	int time1;
}timet[100];

void my_err(char*string, int line)
{
    fprintf(stderr,"line: %d ",line);
    perror(string);
    exit(1);
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
	printf_color(name);
	for(i=0;i<len; i++)
	{
		printf(" ");
	}
	printf("  ");
	g_leave_len -= (g_maxlen + 2);

}

int printf_color(char name[])
{
    struct stat buf;
    if(lstat(name,&buf)==-1)
    {
            my_err(name,__LINE__);
    }
    if(S_ISLNK(buf.st_mode))             //st_mode中获取文件类型（通过使用ＰＯＳＩＸ定义的一系列宏）
    {
        printf("\033[1;36m%s\033[0m  ",name);//|符号链接文件
        return 0;
    }
    if(S_ISDIR(buf.st_mode))
    {
        printf("\033[1;34m%s\033[0m  ",name);//d目录文件
        return 0;
    }
    if(S_ISCHR(buf.st_mode)||S_ISBLK(buf.st_mode))
    {
         printf("\033[1;33m%s\033[0m  ",name);// 字符设备文件//设备文件
         return 0;
    }
    if(buf.st_mode& S_IXUSR)
    {
         printf("\033[1;32m%s\033[0m  ",name);//x 可执行文件
         return 0;
    }
    printf("\033[1;37m%s\033[0m  ",name);
    return 0;
}
int main(int argc,char **argv)
{
	int i=0,j;
	int zl=0,ml=0;
        if(argc==1)     //不带参数查看当前目录。
        {
                flag=0;
                display_ls(".",flag);
        }
	else
	{
		for(i=1;i<argc;i++)
			if(argv[i][0]=='-')
			{
				strcpy(command[zl],argv[i]);
				zl++;
				
			}
			else
			{
				
				strcpy(diirectory[ml],argv[i]);
				ml++;
			}
		if(ml==0)//没有目录全是指令
		{
			strcpy(diirectory[ml],".");	
			ml++;
		}			
	}

	if(zl==0)//没有输入指令 全是目录
	{
		flag=0;
		for(i=0;i<ml;i++)
		{
			printf("%s:\n",diirectory[i]);
		//	printf_color(diirectory[i]);

			display_ls(diirectory[i],flag);
		}		
	}	
	if(zl==1)//只有一个指令
	{
		
		for(i=ml-1;i>=0;i--)
		{
			name[100]='\0';
			t=0;
			ftname[100][100]='\0';
		//	printf_color(diirectory[i]);
			
			printf("%s:\n",diirectory[i]);
			if(strcmp(command[0],"-a")==0)
                	{
                        	flag=1;
                        	display_ls(diirectory[i],flag);
                	}
			if(strcmp(command[0],"-l")==0)
                	{
                        	flag=2;
                        	display_ls(diirectory[i],flag);
                	}
			if(strcmp(command[0],"-u")==0)
			{
                                flag=4;
                                display_ls(diirectory[i],flag);
                        }
			if(strcmp(command[0],"-R")==0)
			{
                                flag=7;
                                display_ls(diirectory[i],flag);
                        }

		}
		
	}

	if(zl==2)//有两个指令任何组合
	{
		
		for(i=ml-1;i>=0;i--)
		{
			name[100]='\0';
			t=0;
			ftname[100][100]='\0';
			printf("%s:\n",diirectory[i]);
//			printf_color(diirectory[i]);
			
			if((strcmp(command[0],"-l")==0&&strcmp(command[1],"-a")==0)||(strcmp(command[1],"-l")==0&&strcmp(command[0],"-a")==0))
			{
                        	flag=3;
                        	display_ls(diirectory[i],flag);
                	}			
			if((strcmp(command[0],"-u")==0&&strcmp(command[13],"-a")==0)||(strcmp(command[1],"-u")==0&&strcmp(command[0],"-a")==0))
			{
                        	flag=5;
                        	display_ls(diirectory[i],flag);
                	}
			if((strcmp(command[0],"-u")==0&&strcmp(command[1],"-l")==0)||(strcmp(command[1],"-u")==0&&strcmp(command[0],"-l")==0))
			{
                        	flag=6;
                        	display_ls(diirectory[i],flag);
                	}
			if((strcmp(command[0],"-R")==0&&strcmp(command[1],"-l")==0)||(strcmp(command[1],"-R")==0&&strcmp(command[0],"-l")==0))
			{
                        	flag=8;
                        	display_ls(diirectory[i],flag);
                	}
			if((strcmp(command[0],"-R")==0&&strcmp(command[1],"-a")==0)||(strcmp(command[1],"-R")==0&&strcmp(command[0],"-a")==0))
			{
                        	flag=9;
                        	display_ls(diirectory[i],flag);
                	}
		}
			
	}

}


/*遍历目录*/
void display_ls(char dirname[],int flag)
{
	int in=0;
	f=0;
        DIR *dir_ptr;
	struct stat buf;
        struct dirent *direntp;
	for(in=0;ftname[in][0]!='\0';in++)
			memset(ftname,0,100);

	 if(lstat(dirname,&buf)<0)           //获取文件/目录属性并赋值给buf
                     my_err("dirname",__LINE__);
         if(S_ISREG(buf.st_mode))        //判断是否为一般文件，是返回真
	{
	//	printf_color("%s\n",dirname);
	        display_single(dirname);
		printf("\n");
		
		strcpy(ftname[0],dirname);
	}		
        if((dir_ptr=opendir(dirname))==NULL)//opendir打开一个目录
                my_err("dirname",__LINE__);
        else
        {	
                strcpy(name,dirname);
                while((direntp=readdir(dir_ptr))!=NULL)//readdir返回目录中的当前项
                {
                        switch(flag)
                        {
                                case 0:
				case 2:
				case 4:
				case 6:
				case 7: if(direntp->d_name[0]=='.')
                                                continue;
                                            strcpy(ftname[f],direntp->d_name);
						f++;
					        break; 
					                          
				case 8:
					flagRl=1;
                       			if(direntp->d_name[0]=='.')
                                                continue;
                                            strcpy(ftname[f],direntp->d_name);
						f++;
					        break; 

                                case 1:
				case 3: 
                                case 5:
				case 9:
                          		strcpy(ftname[f],direntp->d_name);
					f++;
					break;
                        }

                }
                closedir(dir_ptr);
        }
	strcpy(ftname[f],"\0");
	if(flag==7||flag==8||flag==9)
		display_sort_R(f);
  //              list_dir(ftname);
         display_sort();
        if(flag==2||flag==3||flag==4||flag==5||flag==6)
                display_do_stat();


}
/*排序*/
void display_sort()
{
     int i=0,j=0;
        for(i=0;ftname[i][0]!='\0';i++)
                for(j =i+1 ;ftname[j][0]!='\0'; j++)
                {
                        if(strcmp(ftname[i],ftname[j])>0)
                       {
                               char t[100] ;
                               strcpy(t,ftname[i]) ;
                               strcpy(ftname[i],ftname[j]) ;
                               strcpy(ftname[j],t) ;
                        }

                }
        if(flag==0||flag==1)
        {

                for(i=0;ftname[i][0]!='\0';i++)
               
                 //      printf_color("%-10s",ftname[i]);
                       display_single(ftname[i]);
              
        		printf("\n");
		
        }
}

void do_sort1(int count)
{
	char maxLetter[256][20]={'\0'};
	char temp[100];
        int i,j,k=0;
	for(i=0;i<count;i++)
		strcpy(maxLetter[i],ftname[i]);
	printf("\n");
	for(i=0;i<count;i++)
		if(ftname[i][0]>'A'&&ftname[i][0]<'Z')
		{
			ftname[i][0]=ftname[i][0]+32;
		}
        for(i=0;i<count;i++)
                for(j=i+1;j<count;j++)
                {
                        if(strcmp(ftname[i],ftname[j])>0)
                        {
                                strcpy(temp,ftname[i]);
                                strcpy(ftname[i],ftname[j]);
                                strcpy(ftname[j],temp);
                        }
                }	
	for(i=0;i<count;i++)
		for(j=0;j<count;j++)
			if(ftname[i][0]==maxLetter[j][0])
			{
				if(strcmp(ftname[i],maxLetter[i])==0)
					strcpy(ftname[i],maxLetter[i]);
			}
			else if(ftname[i][0]==(maxLetter[j][0]+32))
			{
				ftname[i][0]=ftname[i][0]-32;
				 if(strcmp(ftname[i],maxLetter[i])==0)
					strcpy(ftname[i],maxLetter[i]);
			}

        if(flag==1||flag==0)
        {
                for(i=0;i<count;i++)
                {
                 //       display_single("%-30s",ftname[i]);
                        display_single(ftname[i]);
		 
                        if((i+1)%4==0)
                                printf("\n");
		 }
        }
        printf("\n");
}

/*-R功能实现*/
void display_sort_R(int ff)
{
        struct stat buf;
        struct dirent *pdr;
        DIR *dir_ptr1;
        char name3[100][100];
        //struct dirent *direntp;
        char temname1[100]=" ";
        int i=0,j=0;
        char temname[100]=" ";
        strcpy(temname1,name);
        for(i=0;i<ff;i++)
         {    for(j =i+1 ;ftname[j][0]!='\0'; j++)

                     if(strcmp(ftname[i],ftname[j])>0)
                     {
                               char t[100] ;
                               strcpy(t,ftname[i]) ;
                               strcpy(ftname[i],ftname[j]) ;
                               strcpy(ftname[j],t) ;
                      }
        }
	 
	if(flagRl==1)  //-Rl
		display_do_stat();
	else
	{
		for(i=0;i<ff;i++)
	   	//	display_single("%-10s",ftname[i]);
                        display_single(ftname[i]);
			   
     		printf("\n");
	}
		
        for(i=0;i<ff;i++)
        {
                strcat(name,"/");
                strcat(name,ftname[i]);
                strcpy(name3[i],name);
                strcpy(name,temname1);
        }
         for(i=0;i<ff;i++)
        {
                if(lstat(name3[i],&buf)<0)           //获取文件/目录属性并赋值给buf
                       my_err("name3[i]",__LINE__);

                 if(S_ISDIR(buf.st_mode))        //判断是否为目录，是返回真
                {
        		printf("%s:\n",name3[i]);
                //        display_single(name3[i]);

			
                        flag=7;
                        display_ls(name3[i],flag);
                }
        }
        //遍历子目录
	printf("\n");
}


/***/
void display_do_stat ()
{

        int i;
	char temname[100]=" ";
	strcpy(temname,name);
        struct stat info;
        display_sort();
        for(i=0;ftname[i][0]!='\0';i++)
	{
		strcat(name,"/");
	        strcat(name,ftname[i]);
	   if(stat(name,&info)!=-1)
	   {
			display_count_byt(name,ftname[i],&info);
			if(flag==2||flag==3||flag==8)
				display_stat_info(name,ftname[i],&info);
			strcpy(name,temname);
	   }	
	}
	if(flag==4||flag==5||flag==6)
		display_sortbytime();
         if(flag==6||flag==3||flag==2||flag==8)
        {       printf("总容量:%d\n",allbyt*4);
                allbyt=0;
        }

}
/*统计总用量 -l头行显示总用量*/
void display_count_byt(char *filename2,char *filename1,struct stat *buf)
{
	
	strcpy(timet[t].name2,filename2);
	strcpy(timet[t].name1,filename1);
	timet[t].time1=buf->st_atime;
	t++;

	 if(flag==6||flag==3||flag==2||flag==8)
	{
		
		if(buf->st_size%4096!=0)
			allbyt+=(buf->st_size/4096)+1;
			else	
				allbyt+=buf->st_size/4096;

	} 
}

/*根据最后访问时间排序 -u指令的功能*/
void display_sortbytime()
{
	char yname[100];
	int i,j,t;
	struct stat info;
	for(i=0;i<t;i++)
		for(j=i+1;j<t;j++)
		{
			if(timet[i].time1 < timet[j].time1)
                       {
                                t=timet[i].time1;
				timet[i].time1 =timet[j].time1 ;
				timet[j].time1=t;

				strcpy(yname,timet[i].name1);
				strcpy(timet[i].name1,timet[j].name1);
				strcpy(timet[j].name1,yname);

				strcpy(yname,timet[i].name2);
				strcpy(timet[i].name2,timet[j].name2);
				strcpy(timet[j].name2,yname);

                        }

		}

	for(i=0;i<t;i++)
	{
		
		stat(timet[i].name2,&info);	
		if(flag==6)
			display_stat_info(timet[i].name2,timet[i].name1,&info); //如果是 -lu/-ul时（需要详细打印）
		else
		//	display_single("%s\t",timet[i].name1); //不用详细打印
			display_single(timet[i].name1); //不用详细打印
			
	}
	printf("\n");

}

/*输出详细信息列表 （包扩下边其余函数）-l功能实现*/
void display_stat_info(char *fname,char *fname1,struct stat *buf)
{
    char *uid_to_name(),*gid_to_name();
    char modestr[10];
    mode_detail(buf->st_mode,modestr);
    printf("%s",modestr);
    printf("%4d\t",buf->st_nlink);
    printf("%-8s\t",uid_to_name(buf->st_uid));
    printf("%-8s\t",gid_to_name(buf->st_gid));
    printf("%8ld\t",buf->st_size);
    printf("%.12s\t",4+ctime(&buf->st_mtime));
    printf("%s",fname1);
    printf("\n");
}

/*转化为字符类的访问权利*/
void mode_detail(int mode,char str[])
{

        strcpy(str,"----------");

        if(S_ISDIR(mode))  str[0]='d';//是否目录

        if(S_ISCHR(mode))  str[0]='c';//是否字符装置文件

        if(S_ISBLK(mode))  str[0]='b';//是否为先进先

        if(mode&S_IRUSR)  str[1]='r';//文件所有者可读权限

        if(mode&S_IWUSR)  str[2]='w';//文件所有者可写权限

        if(mode&S_IXUSR)  str[3]='x';//文件所有者可执行权限

	if(mode&S_IXUSR&&mode&S_ISUID)  str[3]='s';//设置了用户权限的x为并且设置了SUID,显示为s
	if(!mode&S_IXUSR&&mode&S_ISUID)  str[3]='S';//没有设置用户权限的x为并且设置了SUID,显示为s

        if(mode&S_IRGRP)  str[4]='r';//用户组

        if(mode&S_IWGRP)  str[5]='w';

        if(mode&S_IXGRP)  str[6]='x';

	if(mode&S_IXGRP&&mode&S_ISGID)  str[6]='s';//设置了用户组权限的x为并且设置了SUID,显示为s
	if(!mode&S_IXGRP&&mode&S_ISGID)  str[6]='S';//没有设置用户组权限的x为并且设置了SUID,显示为s



        if(mode&S_IROTH)  str[7]='r';//其他用户

        if(mode&S_IWOTH)  str[8]='w';

  	if(mode&S_IXOTH)  str[9]='x';

//	if(mode&S_ISVTX)  str[9]='t';//设置了冒险位
}

/*打印所有者用户明名和所有者组名*/

char *uid_to_name(uid_t uid)

{

        struct passwd *getpwuid(), *pw_ptr;

        static char numstr[10];

        if((pw_ptr=getpwuid(uid))==NULL)

        {

                sprintf(numstr,"%d",uid);

                return numstr;

        }

        else

                return pw_ptr->pw_name;

}


char *gid_to_name(gid_t gid)
{

        struct group *getgrgid(), *grp_ptr;

        static char numstr[10];

        if((grp_ptr=getgrgid(gid))==NULL)

          {

               sprintf(numstr,"%d",gid);
                return numstr;
        }
        else
               return grp_ptr->gr_name;

}
