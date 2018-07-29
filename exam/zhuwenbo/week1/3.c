#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct strange 
{
 char a, b, c, d, e, zero;
};
int main(int argc, char *argv[])
{
	 struct strange xp[3];                 //该结构体每个结构体大小为6字节
 	 char characters[6] = "abcde";

 for (int i = 0; i < 6; i++)
 {
 	*((char *)(xp + 0) + i) = characters[i];  //首先将&xp[0]转换为char类型,地址每次增长一字节，将"abcde"分别赋值给结构体中的a,b,c,d,e.
 }
 strcpy((char *)&xp[1], (char *)characters);  //先将&xp[1]转换为char*,然后将charactes的值复制到xp[1]中.
 memcpy(&xp[2], characters, sizeof(struct strange));            //将character地址为起始，连续将之后六个字节中的值复制到&xp[2]结构体的中
 printf("%zu\n", strlen((char *)characters));                   //测量字符数组characters的长度
 for (int i = 0; i < 3; i++) {
 printf("%c\n", xp[i].e);
 }
}
//输出结果为 5 e e e
