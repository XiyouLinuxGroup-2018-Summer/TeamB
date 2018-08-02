#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dir
{
	int num;
	int step;
	struct Dir *next;
} dir;


void push(dir **rear, int num, int step)
{
	dir *t = (dir *)malloc(sizeof(dir));
	t->num = num;
	t->step = step;
	t->next = NULL;
	(*rear)->next = t;
	*rear = t;
}

dir *pop(dir **top)
{
	dir *t = *top;
	*top = (*top)->next;
	return t;
}


int main(void)
{
	int m, n;
	int i;
	int count = 0;
	int symbal[110000] = {0};
	
	while (scanf("%d%d", &m, &n) != EOF)
	{
		dir *head = (dir *)malloc(sizeof(dir)),
			*rear = head;

		head->num = m;
		head->step = 0;
		head->next = NULL;

		dir *p;
		for (dir *t;; t = pop(&head), free(t))
		{
			p = head;
			if(p->num == n)
			{
				break;
			}
			if(p->num < 0 || p->num > 100000 || symbal[p->num])
			{
				continue;
			}

			symbal[p->num] = 1;
	
			if (p->num < n) {
				push(&rear, p->num + 1, p->step + 1);
				push(&rear, p->num * 2, p->step + 1);
			}
			push(&rear, p->num - 1, p->step + 1);
		}

		printf("%d\n", p->step);

		while (head != NULL)
		{
			dir *t = pop(&head);
			free(t);
		}
		memset(symbal, 0, sizeof(symbal));
	}
	return 0;
}
