#include<stdio.h>
#include<stdlib.h>
typedef struct num{
	int x;
	int y;
}cord;
cord a[25];
int count = 0;
void way(int ar[][7],int x,int y,int flag)
{
	a[count].x = x;
	a[count].y = y;
	count++;
	int i;
	if(x == 4 && y == 4) {
		for(i = 0;i < count;i++)
			printf("(%d, %d)\n",a[i].x,a[i].y);
		exit(1);
	}
	if(ar[x][y+1] == 0 && flag != 3)			//right
		way(ar,x,y+1,0);						
	if(ar[x+1][y] == 0 && flag != 2)			//down
		way(ar,x+1,y,1);
	if(ar[x-1][y] == 0 && flag != 1)			//up
		way(ar,x-1,y,2);
	if(ar[x][y-1] == 0 && flag != 0)			//left
		way(ar,x,y-1,3);
	a[count].x = 6;
	a[count].y = 6;
	count--;
	
}
int main(void)
{
	int maze[7][7];
	int i,j;
	
	for(i = 0;i < 7;i++)
		for(j = 0;j < 7;j++)
			maze[i][j] = 1;

	for(i = 0;i < 5;i++)
		for(j = 0;j < 5;j++) 
			scanf("%d",&maze[i][j]);
	
	way(maze,0,0,0);
	return 0;
}
