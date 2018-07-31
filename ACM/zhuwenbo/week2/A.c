#include<stdio.h>
#include<stdlib.h>

//迷宫问题

struct path
{
	int x;            //横座标
	int y;  		  //纵座标
	struct path *next;
};

typedef struct Link
{
	struct path *head;
	struct path *rear;
}link;

void push(link *que, int x, int y)
{
	struct path *p;
	p = (struct path*)malloc(sizeof(struct path));
	p->x = x;
	p->y = y;
	que->rear->next = p;
	que->rear = p;
	que->rear->next = NULL;
}

void pop(link *que)
{
	struct path *temp;
	temp = (struct path*)malloc(sizeof(struct path));
	temp = que->rear;
	free(temp);
	return;
}
void pop2(link *que, int *x, int *y)
{
	struct path *temp;
	temp = que->head->next;
	que->head->next = temp->next;      //将temp结点出队
	*x = temp->x;
	*y = temp->y;
	free(temp);
}

void r(int a[5][5], int x, int y, link *que)
{
	if( x < 5 && x >= 0 && y < 5 && y >= 0)
	{
		if(x == 4 && y == 4)
			return;
		if(a[x][y + 1] == 0 && (y + 1) < 5)
		{
			y++;
			push(que, x, y);
			r(a, x, y, que);
			return;
		}
	    if(a[x + 1][y] == 0 && (x + 1) < 5)
		{
			x++;
			push(que, x, y);
			r(a, x, y, que);
			return;
		}
	    if(a[x - 1][y] == 0 && x - 1 >= 0)
		{
			a[x][y] = 1;
			pop(que);
			x--;
			r(a, x, y, que);
			return;
		}
	    if(a[x][y - 1] == 0 && y - 1 >= 0)
		{
			a[x][y] = 1;
			pop(que);
			y--;
			r(a, x, y, que);
			return;
		}
	}
}

int main(void)
{
	int a[5][5];
	int i, j;
	int x, y;
	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 5; j++)
		{
			scanf("%d", &a[i][j]);
		}
	}
	link *top;
	top = (link *)malloc(sizeof(link));
	top->head = top->rear = (struct path *)malloc(sizeof(struct path));
	r(a, 0, 0, top);
	printf("(0, 0)\n");
	while(top->head->next != top->rear)
	{
		pop2(top, &x, &y);
		printf("(%d, %d)\n", x, y);
	}
	printf("(4, 4)\n");
	return 0;
}
