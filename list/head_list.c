#include <stdlib.h>
#include <stdio.h>

#define LEN 10

struct node;
typedef void (*handle)(struct node *);
typedef int Type;
struct node {
	Type data;
	struct node *next;
};

struct node *create_node(Type data)
{
	struct node *node;

	node = malloc(sizeof(*node));
	if (node == NULL)
		return node;

	node->data = data;
	node->next = NULL;

	return node;
}

int insert_node(struct node *head, Type data)
{
	struct node *node;

	node = create_node(data);
	// ...

	node->next = head->next;
	head->next = node;

	return 0;
}

int create_list(struct node *head, int len)
{
	int i;

	for (i = 0; i < len; i++)
		insert_node(head, i);

	return 0;
}

int reverse_list1(struct node *head)
{
	struct node *sp, *cur;

	sp = NULL;

	while (head->next) {
		cur = head->next;
		head->next = head->next->next;

		cur->next = sp;
		sp = cur;
	}

	head->next = sp;

	return 0;
}

int reverse_list2(struct node *head)
{
	struct node *cur, *next;

	if (NULL == head->next || NULL == head->next->next)
		return 0;

	cur = head->next;
	next = cur->next;

	cur->next = NULL;
	head->next = next;

	reverse_list2(head);
	next->next = cur;

	return 0;
}

int reverse_list3(struct node *head)
{
	struct node *last, *tmp;

	if (NULL == head->next || NULL == head->next->next)
		return 0;

	tmp = head->next;

	while (tmp->next->next)
		tmp = tmp->next;

	last = tmp->next;
	tmp->next = NULL;

	reverse_list3(head);
	last->next = tmp;
	head->next = last;

	return 0;
}
struct node *create_head()
{
	struct node *node;

	node = malloc(sizeof(*node));
	if (node == NULL)
		return node;

	node->next = NULL;

	return node;

}

int transfer(struct node *head, handle func)
{
	struct node *cur, *next;

	cur = head->next;
	while (cur) {
		next = cur->next;
		func(cur);
		cur = next;
	}

	return 0;
}

void show_data(struct node *node)
{
	printf("data = %d\n", node->data);
}

int main()
{
	struct node *head;

	head = create_head();
	// ...

	create_list(head, LEN);
	transfer(head, show_data);

	reverse_list3(head);
	transfer(head, show_data);

	return 0;
}
