#include<stdio.h> 
#include<string.h>
int main()
{
    int n,large;
    int i,j;
    char a[100]; //第一次在这里出错，理由是我写的a[10]空间过于小，在其后进行后台其他字符串测试时，溢出出错
    int flag;
    scanf("%d",&n);
    for(i=0; i<n; i++)
    {
        flag=0;  //一定要注意在有++的循环中最开始初始化
        scanf("%s",a);
        large=strlen(a);
        for(j=0;j<large/2; j++)
        {
            if(a[j]==a[large-1-j])
            flag++;
        }
        if(flag==large/2)
        printf("yes\n");      
        else
        printf("no\n");
        
    }
}
