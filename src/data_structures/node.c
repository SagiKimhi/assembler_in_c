#include <data_structures/node.h>

Node *newNode()
{
	Node *newp = NULL;

	if (!(newp = (Node *) malloc(sizeof(Node))))
		return NULL;

	newp->data = NULL;
	newp->dataSize = 0;
	newp->next = NULL;

	return newp;
}

void deleteNode(Node *node)
{
	if (!node)
		return;
	free(node->data);
	free(node);
}

Node *setData(Node *node, const void *data, size_t dataSize)
{
	if (!node || !data || !dataSize)
		return NULL;
	
	if (!(node->data = realloc(node->data, dataSize)))
		return NULL;

	memmove(node->data, data, dataSize);
	node->dataSize = dataSize;

	return node;
}
