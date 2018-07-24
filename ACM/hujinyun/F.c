#include<stdio.h> 
#include<string.h>
int main()
{
    int n;
    char a[100];
    int flag=0;
    int i;
    scanf("%d",&n);
    getchar();
         while(n--)
    {
        flag=0;
        scanf("%s",a);
 //       gets(a);
        for(i=0; i<strlen(a); i++)
        {
            if(a[i]>='0' && a[i]<='9')
            flag++;
        }
        printf("%d\n",flag);
    }  
    return 0;
}
