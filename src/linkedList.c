#include <linkedList.h>

/* 	newElement: Returns a pointer to a new Node element, or null if not enough
	space could be allocated in memory. */
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

List *newList()
{
	List *list = NULL;

	if (!(list = (List *) malloc(sizeof(List))))
		return NULL;

	list->head = NULL;
	list->tail = NULL;

	return list;
}

/* deleteNode: removes an element from the list */
void deleteNode(Node *node)
{
	if (!node)
		return;
	free(node->data);
	free(node);
}

/*	deleteList: deletes the entire list */
void deleteList(List *list)
{
	Node *temp;
	if (!list)
		return;
	while (list->head != NULL) {
		temp = list->head;
		list->head = temp->next;
		deleteNode(temp);
	}
	free(list);
}

/* addToStart: add an element to the begining of the list. */
Node *insertData(Node *node, const void *data, size_t dataSize)
{
	if (!node || !data || !dataSize)
		return NULL;
	
	if (!(node->data = realloc(node->data, dataSize)))
		return NULL;

	memmove(node->data, data, dataSize);
	node->dataSize = dataSize;

	return node;
}

List *addToFront(List *list, const void *data, size_t dataSize)
{
	Node *newp = NULL;

	if (!list)
		return NULL;
	
	if (!(newp = newNode()) || !insertData(newp, data, dataSize))
		return NULL;

	newp->next = list->head;
	list->head = newp;

	return list;
}

List *addToEnd(List *list, const void *data, size_t dataSize)
{
	Node *newp = NULL;

	if (!list)
		return NULL;
	
	if (!(newp = newNode()) || !insertData(newp, data, dataSize))
		return NULL;

	list->tail->next = newp;
	list->tail = newp;

	return list;
}

void *search(List *list, const void *data, size_t dataSize)
{
	Node *ptr=NULL;

	if (!list)
		return NULL;

	ptr = list->head;
	while (ptr != NULL) {
		if (ptr->dataSize == dataSize && !memcmp(ptr->data, data, dataSize))
			return ptr;
	}
	return NULL;
}

/* fprintList: prints the text contents of the list's elements onto a file. */
void fprintList(FILE *fp, List *list, void (*fprintData)(void *))
{
	Node *ptr=NULL;

	if (!fp || !list)
		return;
	
	ptr = list->head;
	while (ptr != NULL) {
		(*fprintData)(ptr->data);
		ptr = ptr->next;
	}
}
