/*2018-7-24 周二 */
/*my_chmod.c 改变文件访问权限 */
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
	//检查参数
	if(argc < 3) {
		printf("错误引用");
		return 0;
	}

	struct stat buf;
	if(argv[1][0] >= '0' && argv[1][0] <= '9') {
		int		mode;	//权限
		int		mode_u;	//所有者权限
		int		mode_g;	//用户组权限
		int		mode_o;	//其他用户权限

		//获取命令行参数
		mode_u = argv[1][0] - '0';
		mode_g = argv[1][1] - '0';
		mode_o = argv[1][2] - '0';
		mode = mode_u*8*8 + mode_g*8 + mode_o;
		if(chmod(argv[2],mode) == -1) 
			printf("改变失败");
	}
	else {
		if(lstat(argv[2],&buf) == -1) {
			printf("read data error");
			return 0;
		}

		switch(argv[1][0]) {
			case 'u':	switch(argv[1][1]) {
							case '+':	switch(argv[1][2]) {
											case 'x':
												buf.st_mode |= S_IXUSR;
												break;
											case 'w':
												buf.st_mode |= S_IWUSR;
												break;
											case 'r':
												buf.st_mode |= S_IRUSR;
												break;
										}
										break;
							case '-':	switch(argv[1][2]) {
											case 'x':
												buf.st_mode &= (~S_IXUSR);
												break;
											case 'w':
												buf.st_mode &= (~S_IWUSR);
												break;
											case 'r':
												buf.st_mode &= (~S_IRUSR);
												break;
										}
										break;
						}
						break;
			case 'g':	switch(argv[1][1]) {
							case '+':	switch(argv[1][2]) {
											case 'x':
												buf.st_mode |= S_IXGRP;
												break;
											case 'w':
												buf.st_mode |= S_IWGRP;
												break;
											case 'r':
												buf.st_mode |= S_IRGRP;
												break;
										}
										break;
							case '-':	switch(argv[1][2]) {
											case 'x':
												buf.st_mode &= (~S_IXGRP);
												break;
											case 'w':
												buf.st_mode &= (~S_IWGRP);
												break;
											case 'r':
												buf.st_mode &= (~S_IRGRP);
												break;
										}
										break;
						}
						break;
			case 'o':	switch(argv[1][1]) {
							case '+':	switch(argv[1][2]) {
											case 'x':
												buf.st_mode |= S_IXOTH;
												break;
											case 'w':
												buf.st_mode |= S_IWOTH;
												break;
											case 'r':
												buf.st_mode |= S_IROTH;
												break;
										}	
										break;
							case '-':	switch(argv[1][2]) {
											case 'x':
												buf.st_mode &= (~S_IXOTH);
												break;
											case 'w':
												buf.st_mode &= (~S_IWOTH);
												break;
											case 'r':
												buf.st_mode &= (~S_IROTH);
												break;
										}
										break;
						}
		}
	}
	if(chmod(argv[2],buf.st_mode) == -1) 
		printf("修改失败");
	return 0;
}

