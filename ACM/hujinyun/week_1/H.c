#include<stdio.h>
#include<string.h>
int main()
{
    int n;
    char a[100];
    scanf("%d",&n);
    int len;
    int i;
    while(n--)
    {
       gets(a); //scanf可能导致空格输入问题吧
        getchar();
        if(!( (a[0]>='A' && a[0]<='Z') || (a[0]>='a' && a[0]<='z') || a[0]=='_' ))
        {
            printf("no\n");
        }
        else
        {
            len=strlen(a);
     
            for(i=1;i<len; i++)
            {
                if( !((a[i]>='A'&&a[i]<='Z') ||(a[i]>='a'&&a[i]<='z') || (a[i]>='0'&&a[i]<='9')||a[i]=='_' ))
                {
                    printf("no\n");
                    break;
                }
            }
            if(i==len)
            printf("yes\n");

        }
    }
}