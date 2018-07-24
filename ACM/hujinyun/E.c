#include<stdio.h> 
int main()
{
    int n;
    float a,b;
    char op;
    float num;
    int i;
    scanf("%d",&n);
    getchar();
    if(n>0 && n<1000)
    {
         while(n--)
    {
        scanf("%c",&op);
        scanf("%f %f",&a,&b);
        if( (a>0 && a<10000) && (b>0 && b<10000))
        {
             getchar(); 
            if(op == '+')
            printf("%.0f\n",a+b);
            if(op == '-')
            printf("%.0f\n",a-b);
            if(op == '*')
            printf("%.0f\n",a*b);
            if(op == '/')
            {
            if((int)(a/b) == a/b)
            {
                printf("%.0f\n",a/b);
            }
            
            else
            {
                num = a/b;
                printf("%.2f\n",num);
            }           
            }
        }
        else
        {return 0;}
       
    }
    }
   
    return 0;
}


