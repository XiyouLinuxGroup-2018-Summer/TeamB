#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct tag_unimportant {
    char *name;
    int num;
} A;
void func(A **a)
{
    (*a) = (A *)malloc(sizeof(A));
    (*a)->name = (char *)malloc(sizeof(char)*30);//只申请了A类型的空间，a->name无指向空间
    strcpy((*a)->name, "Xiyou Linux Group");
    (*a)->num = 20180728;
}
int main(int argc, char *argv[])
{
    A *a;
    func(&a);			//未传入a的地址，只有形参发生了变化
    printf("%s %d\n", a->name, a->num);
	free(a->name);
	free(a);
    return 0;
}
