#include <linkedList.h>

List *newList()
{
	List *list = NULL;

	if (!(list = (List *) malloc(sizeof(List))))
		return NULL;

	list->head = NULL;
	list->tail = NULL;

	return list;
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

List *addToFront(List *list, const void *data, size_t dataSize)
{
	Node *newp;

	if (!list)
		return NULL;
	
	if (!(newp = newNode()) || !setData(newp, data, dataSize))
		return NULL;

	if (!list->head) {
		list->head = list->tail = newp;
		return list;
	}

	newp->next = list->head;
	list->head = newp;

	return list;
}

List *addToEnd(List *list, const void *data, size_t dataSize)
{
	Node *newp = NULL;

	if (!list)
		return NULL;
	
	if (!(newp = newNode()) || !setData(newp, data, dataSize))
		return NULL;

	if (!list->head) {
		list->head = list->tail = newp;
		return list;
	}

	list->tail->next = newp;
	list->tail = newp;

	return list;
}

void *searchList(List *list, const void *data, size_t dataSize)
{
	Node *ptr=NULL;

	if (!list)
		return NULL;

	ptr = list->head;
	while (ptr != NULL) {
		if (ptr->dataSize == dataSize && !memcmp(ptr->data, data, dataSize))
			return ptr->data;
	}
	return NULL;
}

/* fprintList: prints the text contents of the list's elements onto a file. */
void fprintList(FILE *fp, List *list, void (*fprintData)(FILE *, void *))
{
	Node *ptr;
	int count;

	if (!fp || !list || !fprintData)
		return;
	
	ptr = list->head;

	for (count = 1; ptr != NULL; count++, ptr = ptr->next) {
		fprintf(fp, "Node #%d: ", count);
		(*fprintData)(fp, ptr->data);
	}
}
