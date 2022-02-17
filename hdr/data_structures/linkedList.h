#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H
#include <libraries.h>
#include <data_structures/node.h>

typedef struct list {
	Node *head, *tail;
} List;


List *newList();
List *addToFront(List *list, const void *data, size_t dataSize);
List *addToEnd(List *list, const void *data, size_t dataSize);
void *searchList(List *list, const void *data, size_t dataSize);
void fprintList(FILE *fp, List *list, void (*fprintData)(FILE *, void *));
void deleteList(List *list);

#endif
