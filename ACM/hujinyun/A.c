#include<stdio.h> //关键是把输入后的回车过滤掉
                  //三个字母直接定义成三个变量利于操作，不用使用数组了
int main()
{
    char a,b,c;
    char t;
    while((a= getchar())!=EOF)  //注意这里要在EOF前加括号！！！
    {
        b=getchar();
        c=getchar();
        getchar(); //过滤回车
        if(a>b)
        {
            t=a;
            a=b;
            b=t;
        }
        if(a>c)
        {
            t=a;
            a=c;
            c=t;
        }
        if(b>c)
        {
            t=b;
            b=c;
            c=t;
        }
        printf("%c %c %c\n",a,b,c);
    }
   return 0;
}   
