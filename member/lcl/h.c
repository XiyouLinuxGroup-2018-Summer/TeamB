/*************************************************************************
	> File Name: a.c
	> Author: 
	> Mail: 
	> Created Time: 2018年07月24日 星期二 15时49分00秒
 ************************************************************************/
//--------H合法标识符
#include<stdio.h>
#include<string.h>
int f(char num[])
{
    if(strcmp(num,"int")==0)
        return 0;
    else if((strcmp(num,"int")==0)||(strcmp(num,"double")==0)||(strcmp(num,"enum")==0))
        return 0;
    else if((strcmp(num,"float")==0)||(strcmp(num,"char")==0)||(strcmp(num,"long")==0))
        return 0;
    else if((strcmp(num,"short")==0)||(strcmp(num,"signed")==0)||(strcmp(num,"struct")==0))
        return 0;
    else if((strcmp(num,"union")==0)||(strcmp(num,"unsigned")==0)||(strcmp(num,"void")==0))
        return 0;
    else if((strcmp(num,"for")==0)||(strcmp(num,"do")==0)||(strcmp(num,"while")==0))
        return 0;
    else if((strcmp(num,"break")==0)||(strcmp(num,"continue")==0)||(strcmp(num,"if")==0))
        return 0;
    else if((strcmp(num,"else")==0)||(strcmp(num,"goto")==0)||(strcmp(num,"switch")==0))
        return 0;
    else if((strcmp(num,"case")==0)||(strcmp(num,"default")==0)||(strcmp(num,"return")==0))
        return 0;
    else if((strcmp(num,"auto")==0)||(strcmp(num,"extern")==0)||(strcmp(num,"register")==0))
        return 0;
    else if((strcmp(num,"static")==0)||(strcmp(num,"const")==0)||(strcmp(num,"sizeof")==0))
        return 0;
    else if((strcmp(num,"typedef")==0)||(strcmp(num,"volatile")==0))
        return 0;
    else
        return 1;
}
int g(char num[])
{
    int i=1;
    if(((num[0]>=65)&&(num[0]<=90))||((num[0]>=97)&&(num[0]<=122))||num[0]==95)
    {
        for(i=1;i<strlen(num);i++)
        {
            if(((num[i]>=65)&&(num[i]<=90))||((num[i]>=97)&&(num[i]<=122))||num[i]==95||((num[i]>=48)&&(num[i]<=57)))
                ;
            else
            {
                return 0;
                break;
            }
        }
    }
    else
        return 0;
}
int main(void)
{
    int n;
    char num[100];
    scanf("%d",&n);
    getchar();
    while(n--)
    {
        gets(num);
        if(f(num)==0)
            printf("no\n");
        else
        {
            if(g(num)==0)
                printf("no\n");
            else
                printf("yes\n");
        }
    }
}





