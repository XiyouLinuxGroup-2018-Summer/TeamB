#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int count = 0;

void dfs(int line, int dir[], char sym[8][8], int n, int k)
{
	int j;
	if(k == 0)
	{
		count++;
		return;
	}
	if(line >= n)    //防止放入的棋子超出棋盘
		return;
	for(j = 0; j < n; j++)
	{
		if(dir[j] == 0 && sym[line][j] == '#')
		{
			dir[j] = 1;
			dfs(line + 1, dir, sym, n, k - 1);
			dir[j] = 0;
		}
	}
	dfs(line + 1, dir, sym, n, k);
}

int main(void)
{
	char sym[8][8] = {0};
	int n, k;
	int i, j;
	int dir[8] = {0};     
	scanf("%d%d", &n, &k);
	while(n != -1 || k != -1)
	{
		for(i = 0; i < n; i++)
		{
			for(j = 0; j < n; j++)
				scanf(" %c", &sym[i][j]);
		}
		dfs(0, dir, sym, n, k);
		printf("%d\n", count);
		count = 0;
		memset(dir, 0, sizeof(dir));
		memset(sym, '\0', sizeof(sym));
		scanf("%d%d", &n, &k);
	}
	return 0;
}
