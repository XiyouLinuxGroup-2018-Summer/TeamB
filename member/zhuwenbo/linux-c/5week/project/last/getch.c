#include<stdio.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

int getch()//getch实现
{
    int c=0;
    struct termios org_opts, new_opts;
    int res=0;
     res=tcgetattr(STDIN_FILENO, &org_opts);
     assert(res==0);
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    c=getchar();
    res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
    assert(res==0);
    return c;
}
int main()
{
	char passwd[20] = {0};
	int i = 0;
	int ch;
	ch = getch();
	while(ch != '\n')
	{
		if(ch == 127)
		{
			passwd[--i] = '\0';
			printf("\b \b");
			ch = getch();
			continue;
		}
		passwd[i++] = ch;
		printf("*");
		ch = getch();
	}
	printf("passwd = %s\n", passwd);
}
