#include <stdlib.h>
#include <stdio.h>

typedef int Type;
typedef struct list_node {
	struct list_node *next;
	Type data;
} Node, *NodePtr;

NodePtr alloc_node(Type data)
{
	NodePtr node;

	node = malloc(sizeof(*node));
	if (NULL == node)
		return NULL;

	node->data = data;
	node->next = NULL;

	return node;
}

NodePtr insert_node(NodePtr node, Type data)
{
	NodePtr tmp;

	tmp = alloc_node(data);
	if (NULL == tmp)
		return NULL;

	tmp->next = node;
	node = tmp;

	return node;
}

NodePtr create_list(int n)
{
	int i;
	NodePtr first = NULL;

	for (i = 0; i < n; i++)
		first = insert_node(first, i);

	return first;
}

NodePtr reverse(NodePtr first)
{
	NodePtr node, ne;

	if (NULL == first || NULL == first->next)
		return first;

	ne = first->next;

	node = reverse(ne);
	ne->next = first;
	first->next = NULL;

	return node;
}

NodePtr reverse2(NodePtr first)
{
	NodePtr cur, last;

	if (NULL == first || NULL == first->next)
		return first;

	cur = first;
	while (cur->next->next)
		cur = cur->next;

	last = cur->next;
	cur->next = NULL;
	reverse2(first);
	last->next = cur;

	return last;
}

NodePtr iter_stack(NodePtr first)
{
	NodePtr sp, cur;

	sp = NULL;

	while (first) {
		cur = first;
		first = first->next;

		cur->next = sp;
		sp = cur;
	}

	return sp;
}

void transfer(NodePtr first, void (*handle)(NodePtr))
{
	NodePtr tmp;

	while (first) {
		tmp = first;
		first = first->next;
		handle(tmp);
	}
}

void show(NodePtr node)
{
	printf("%d ", node->data);
}

int main()
{
	NodePtr first;

	first = create_list(10);
	transfer(first, show);
	putchar('\n');

	first = iter_stack(first);
	transfer(first, show);
	putchar('\n');

	return 0;
}
