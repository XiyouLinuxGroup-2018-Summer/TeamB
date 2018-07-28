#include<string.h>
#include<stdio.h>
#include<stdlib.h>
struct strange {
    char a, b, c, d, e, zero;
};

int main(int argc, char *argv[])
{
    struct strange xp[3];				//声明一个struct strange类型的数组
    char characters[6] = "abcde";		
    for (int i = 0; i < 6; i++) {
	//将数组characters中的每个元素赋给xp[0]中的每个元素	
        *((char *)(xp + 0) + i) = characters[i];	
    }
	//将字符串"abcde"复制到xp[1]上
    strcpy((char *)&xp[1], (char *)characters);
	//将characters在内存中所存储的"abcde"拷贝到xp[2]
    memcpy(&xp[2], characters, sizeof(struct strange));
	//输出字符串characters的长度
    printf("%zu\n", strlen((char *)characters));
	//输出结构体数组xp中每个元素中的成员e的字符
    for (int i = 0; i < 3; i++) {
        printf("%c\n", xp[i].e);
    }
}
