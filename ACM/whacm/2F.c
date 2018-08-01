#include<stdio.h>
typedef struct a{
	int x;
	int y;
	int flag;
	char data;
}A;
int count = 0;
void Flag(int n,A ar[][n+2],int x,int y)				//递归遍历所有结点，直到无结点可遍历
{
	int i,j;
	ar[x][y].flag = count;							//结点染色，使之变为已遍历的结点
	if(ar[x-1][y-1].data == '@' && ar[x-1][y-1].flag == 0)		//避免重复染色
		Flag(n,ar,x-1,y-1);							
	if(ar[x-1][y].data == '@' && ar[x-1][y].flag == 0)
		Flag(n,ar,x-1,y);		
	if(ar[x-1][y+1].data == '@' && ar[x-1][y+1].flag == 0) 
		Flag(n,ar,x-1,y+1);		
	if(ar[x][y-1].data == '@' && ar[x][y-1].flag == 0)
		Flag(n,ar,x,y-1);		
	if(ar[x][y+1].data == '@' && ar[x][y+1].flag == 0)
		Flag(n,ar,x,y+1);		
	if(ar[x+1][y-1].data == '@'&& ar[x+1][y-1].flag == 0)
		Flag(n,ar,x+1,y-1);		
	if(ar[x+1][y].data == '@' && ar[x+1][y].flag == 0)
		Flag(n,ar,x+1,y);		
	if(ar[x+1][y+1].data == '@' && ar[x+1][y+1].flag == 0)
		Flag(n,ar,x+1,y+1);		
	return;											//无结点可遍历
}
int main(void)
{
	int m,n;
	int i,j;										//循环变量
	while(scanf("%d %d",&m,&n) != EOF && (m != 0 || n != 0) ) {
		A ar[m+2][n+2];
		for(i = 0;i < m+2;i++)
			for(j = 0;j < n+2;j++) {
				ar[i][j].x = i;
				ar[i][j].y = j;
				ar[i][j].flag = 0;
				ar[i][j].data = '*';
			}
		while(getchar() != '\n');
		for(i = 1;i < m+1;i++) {
			for(j = 1;j < n+1;j++) 
				scanf("%c",&ar[i][j].data);
			while(getchar() != '\n');
		}

			
		for(i = 1;i < m+2;i++)
			for(j = 1;j < n+2;j++)
				if(ar[i][j].data == '@')			//找到结点
					if(ar[i][j].flag == 0) {		//未遍历过的结点
						count++;
						Flag(n,ar,i,j);				//递归遍历所有结点并染色
					}
		printf("%d\n",count);
		count = 0;
	}
	return 0;
}


