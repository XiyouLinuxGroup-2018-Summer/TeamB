

标签： Linux系统调用编程

---

##目录的创建和删除
目录的创建可以由mkdir系统调用来实现
其函数原型为
```
#include<sys/types.h>
#include<sys/stat.h>
int mkdir (const char *pathname, mode_t mode);
```
mkdir创建一个新的空目录。空目录中自动创建. 和 .. 目录项
所创建目录的存取许可权，由mode（mode &～ umask）决定。
新创建目录的uid与创建该进程的uid一致。如果父目录设置了st_gid位，则新目录同样也设置st_gid位。
（注：目录被设置st_gid位后，任何用户在此目录下创建的文件组的id与该目录组id相同）。

rmkdir
```
#include<unistd.h>
int rmkdir(const char *pathname)
```
注：只能删除pathname指定的 **空目录**。

<br>
##获取当前目录
每个进程（运行中的程序）都有一个当前工作目录，此目录是搜索所有相对路径名（以/开头）的起点。当前工作目录是是进程的一个属性。getcwd系统调用可以获取进程当前工作目录。
```
#include<unistd.h>
char *getcwd(char *buf, size_t size);
char *get_current_dir_name(void);
char *getwd(char *buf);
```
getcwd会将当前的工作目录绝对路径复制到参数buf所指向的内存空间
参数size为buf的空间大小，注意buf所指的空间要足够大。
进程可以在使用完此字符串后利用free()来释放此空间。
##设置工作目录
使用chdir可以更改当前工作目录
```
#include<unistd.h>
int chdir(const char *path);
int fchdir(int fd);
```
将当前工作目录改为由参数path指定的目录。
fchdir用来将当前工作目录改为由参数fd（文件描述符）
<br>
##获取目录信息

`获取目录信息（只要对目录具有读权限，就可以获取目录信息）
`
opendir
```
#include<stdio.h>
#include<direct.h>
DIR *opendir(const char *name);
```
opendir函数用来打开参数的指定目录， 并返回DIR*形态的目录流（类似于文件操作的文件描述符），接下来对目录的读取和搜索都要用此返回值。
失败返回NULL
<br>

readdir
```
#include<sys/types.h>
#include<dirent.h>
struct dirent *readdir(DIR *dir);
```
readdir用来从参数dir所指向的目录中读取出目录项信息。返回一个struct direct的文件指针。
```
struct dirent
{
    Long d_ino;     //inode number
    off_t d_off;     //offset to this direct
    insigned short d_reclen;   //length of this name
    char d_name [NAME_MAX+1];  //file name(NULL-terminated);
    
}
```
执行成功返回该目录下一个文件的信息
<br>

closedir
```
#include<dirent.h>
#nclude<sys/types>
int close(DIR *dir);
```
关闭参数dir指向的目录

<mark>注意：一定要通过反复调用opendir函数返回的目录流来对文件进行读取搜索等，不然会失效。</mark>