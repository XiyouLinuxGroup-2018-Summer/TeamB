//exec族

//exec函数的作用可以用来让该进程执行别的程序，并且当exec函数执行的程序结束后，该进程也结束.

/*
好像有八个函数


char *argv[]必须以NULL作为结尾

int execve(char *path, char *argv[], char *envp[]);
int execv(char *path, char *envp[]);
int execle(char *path, char *arg, ....);
int execl(char *path, char *arg, ....);
前面四个接受路径名方式调用可执行文件

下面的是调用可执行文件的名程，path路径在PATH中寻找或者用户自己定义
int execvp(char *file, char *argv[]);
int execlp(char *file, char *arg, ....);
int exevp(char *file, char *argvp[]);


*/

