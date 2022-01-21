#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H
#include <libraries.h>

typedef struct node {
	void *data;
	size_t dataSize;
	struct node *next;
} Node;

typedef struct list {
	Node *head, *tail;
} List;

Node *newNode();
List *newList();
void deleteNode(Node *node);
void deleteList(List *list);
Node *insertData(Node *node, const void *data, size_t dataSize);
List *addToFront(List *list, const void *data, size_t dataSize);
List *addToEnd(List *list, const void *data, size_t dataSize);
void *search(List *list, const void *data, size_t dataSize);
void fprintList(FILE *fp, List *list, void (*fprintData)(void *));

#endif
