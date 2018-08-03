#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Time
{
	int s;
	int m;
	int n;
	int step;
	struct Time *next;
}time;

struct dir
{
	int s[101];
	int m[101];
	int n[101];
};

void push(time **rear, int s, int m, int n, int step)
{
	time *temp;
	temp = (time*)malloc(sizeof(time));
	temp->s = s;
	temp->m = m;
	temp->n = n;
	temp->step = step;
	(*rear)->next = temp;
	temp->next = NULL;
	(*rear) = temp;
	(*rear)->next == NULL;
}

time * pop(time **head)
{
	time *temp;
	temp = *head;
	*head  = (*head)->next;
	return temp;
}

int main(void)
{
	int s, m, n;
	scanf("%d%d%d", &s, &m, &n);
	int flag;
	struct dir *symbal = (struct dir*)malloc(sizeof(struct dir));
	memset(symbal->s, 0, 101 * 4);
	memset(symbal->m, 0, 101 * 4);
	memset(symbal->n, 0, 101 * 4);
	while(s != 0 || m != 0 || n != 0)
	{
		time *head, *rear;
		head = rear = (time *)malloc(sizeof(time));
		head->s = s;
		head->m = 0;
		head->n = 0;
		head->step = 0;
		head->next = NULL;
		time *p;
		while(head != NULL)
		{
			p = head;
			if((p->s == p->m && p-> n == 0) || (p->s == p->n && p->m == 0))
			{
				flag = 1;
				break;
			}
			if( !(symbal->s[p->s] == 1 && symbal->m[p->m] == 1 && symbal->n[p->n] == 1) )
			{
				symbal->s[p->s] = 1;
				symbal->m[p->m] = 1;
				symbal->n[p->n] = 1;
				if(p->m != m && p->s > 0 && p->s + p->m >= m)
					push(&rear, p->s -m + p->m, m, p->n, p->step + 1);   //s倒m
				if(p->m != m && p->s > 0 && p->s + p->m < m)
					push(&rear, 0, p->s+p->m, p->n, p->step + 1);

				if(p->s != s && p->m > 0 && p->s + p-> m >= s)         //m倒s
					push(&rear, s, p->m -s + p->s, p->n, p->step + 1);
				if(p->s != s && p->m > 0 && p->s + p->m < s)
					push(&rear, p->m + p->s, 0, p->n, p->step + 1);

				if(p->n != n && p->s > 0 && p->s + p-> n >= n)         //s倒n
					push(&rear, p->s -n + p->n, p->m, n, p->step + 1);
				if(p->n != n && p->s > 0 && p->s + p-> n < n)
					push(&rear, 0, p->m, p->s + p->n, p->step + 1);

				if(p->s != s && p->n > 0 && p->s + p-> n >=s)         //n倒s
					push(&rear, s, p->m, p->n -s + p->s, p->step + 1);
				if(p->s != s && p->n > 0 && p->s + p->n < s)
					push(&rear, p->s + p-> n, p->m, 0, p->step + 1);

				if(p->n != n && p->m > 0 && p->m + p-> n >= n)        //m倒n
				push(&rear, p->s, p->m - n + p->n, n, p->step +1);
				if(p->n != n && p->m > 0 && p->m + p->n < n)
					push(&rear, p->s, 0, p->m + p->n, p->step + 1);

				if(p->m != m && p->n > 0 && p->n + p-> m >= m)         //n倒m
					push(&rear, p->s, m, p->n - m + p->m, p->step + 1);
				if(p->m != m && p->n > 0 && p->n + p->m < m)
					push(&rear, p->s, p->m + p->n, 0, p->step + 1);
			}
			time *t = pop(&head);
			free(t);
		}
		if(flag == 1)
			printf("%d\n", p->step);
		else
			printf("NO\n");
		while(head != NULL)
		{
			time *temp = pop(&head);
			free(temp);
		}
		memset(symbal->s, 0, sizeof(symbal->s));
		memset(symbal->m, 0, sizeof(symbal->m));
		memset(symbal->n, 0, sizeof(symbal->n));
		flag = 0;
		scanf("%d%d%d", &s, &m, &n);
	}	
	return 0;
}
