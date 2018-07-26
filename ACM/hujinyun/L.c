#include<stdio.h>
int main()
{
    char ch;
    int flag=0;
    int p=0;
    while((ch = getchar())!= EOF)
    {
//        flag=0;
        p++;
        if(p == 1)
        flag=1;
        if(flag == 1)
        printf("%c",ch-32);
        else if(flag == 0)
        printf("%c",ch);

        if((ch>='A' && ch<='Z') || (ch>='a' && ch<='z'))
        flag=0;
        else if(ch == '\n' || ch == ' ')
        flag=1;
    }
}