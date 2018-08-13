#include<stdio.h>
#include<malloc.h>
typedef struct tag_unimportant
{
    char *name;
    int num;
}A;

void func(A **a)
{
    a = (A **)malloc(sizeof(*A));
    strcpy(*a->name,"Xiyou Linux Group");
    *a->num = 

}
