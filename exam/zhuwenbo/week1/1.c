#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct tag_unimportant
{
 char *name;
 int num;
} A;
void func(A **a)
{
 *a = (A *)malloc(sizeof(A));
 (*a)->name = (char *)malloc(sizeof(char) * 20);
 strcpy((*a)->name, "Xiyou Linux Group");
 (*a)->num = 20180728;
}
int main(int argc, char *argv[])
{
 A *a;
 func(&a);
 printf("%s %d\n", a->name, a->num);
 return 0;
}


//原因 : 原代码在主函数调用fun函数时没有对结构体指针a初始化，在fun函数里也没有对结构体中的字符指针初始化。
//修改: 将fun函数的结构体指针改成二级指针，然后在主函数中传入结构体指针的地址，并且在fun函数中对结构体中的字符指针进行初始化。
