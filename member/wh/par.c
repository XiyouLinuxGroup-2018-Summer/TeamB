#include <stdio.h>
#include <stddef.h>
#include <time.h>
int main(void)
{
	time_t timer;//time_t就是long int 类型
	struct tm *tblock;
	timer = time(NULL);//这一句也可以改成time(&timer);
	tblock = localtime(&timer);
	printf("Local time is: %s\n",asctime(tblock));
	return 0;
}
