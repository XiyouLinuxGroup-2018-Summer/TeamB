#include<stdio.h>
#include<stdlib.h>
//勘探石油

int direct[8][2] = { 0,1, 1,0, 1,1, 1,-1, 0,-1, -1,-1, -1,0, -1,1};//将可能的八种方向存入数组中

void r(int x, int y, int m, int n, char sign[101][101])
{
	sign[x][y] = '*';
	for (int i = 0; i < 8; i++)                      //判断八种方向
	{
		int newx = x + direct[i][0],
			newy = y + direct[i][1];

		if (newx < 0 || newx >= m || newy < 0 || newy >= n) continue;
		if (sign[newx][newy] == '@')
		{
			r(newx, newy, m, n, sign);
		}
	}
}

int main(void)
{
	int m, n;
	int i, j;
	char sign[101][101] ={0};
	int count = 0;
	scanf("%d%d", &m, &n);
	while(m != 0 || n != 0)
	{
		for(i = 0; i < m; i++)
		{
			for(j = 0; j< n; j++)
			{
				scanf(" %c", &sign[i][j]);
			}
		}
		for(i = 0; i < m; i++)
		{
			for(j = 0; j < n; j++)
			{
				if(sign[i][j] == '@')
				{
					r(i, j, m, n, sign);
					count++;
				}
			}
		}
		printf("%d\n", count);
		count = 0;
		scanf("%d%d", &m, &n);
	}
}


