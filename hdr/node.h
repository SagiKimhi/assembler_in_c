#ifndef _NODE_H
#define _NODE_H
#include <libraries.h>

typedef struct node {
	void *data;
	size_t dataSize;
	struct node *next;
} Node;

Node *newNode();
void deleteNode(Node *node);
Node *setData(Node *node, const void *data, size_t dataSize);

#endif
