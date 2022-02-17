#include <data_structures/wordList.h>

WordList *newWordList()
{
	WordList *newp = (WordList *) malloc(sizeof(WordList));

	if (!newp)
		return NULL;

	newp->head = NULL;
	newp->tail = NULL;

	return newp;
}

WordList *w_addToFront(WordList *list, const char *name, void *data)
{
	WordNode *newp;

	if (!list || !(newp = newWordNode(name, data)))
		return NULL;

	if (!list->head) {
		list->head = list->tail = newp;
		return list;
	}

	newp->next = list->head;
	list->head = newp;

	return list;
}

WordList *w_addToEnd(WordList *list, const char *name, void *data)
{
	WordNode *newp;

	if (!list || !(newp = newWordNode(name, data)))
		return NULL;

	if (!list->head) {
		list->head = list->tail = newp;
		return list;
	}

	list->tail->next = newp;
	list->tail = newp;

	return list;
}

void *w_searchList(WordList *list, const char *name)
{
	WordNode *temp;

	if (!list || !name)
		return NULL;

	temp = list->head;
	while (temp != NULL) {
		if (!strcmp(temp->name, name))
			return temp->data;
		temp = temp->next;
	}

	return NULL;
}

void w_deleteListNode(WordList *list, char *name)
{
	WordNode *prev, *curr;

	if (!list || !list->head || !name)
		return;

	prev = curr = list->head;
	
	if (!list->head->next && !strcmp(list->head->name, name)) {
		list->head = list->tail = NULL;
		deleteWordNode(curr);
		return;
	}

	while (curr != NULL) {
		if (!strcmp(curr->name, name)) {
			prev->next = curr->next;
			deleteWordNode(curr);
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}

void w_deleteList(WordList *list)
{
	WordNode *temp;

	if (!list)
		return;

	while (list->head != NULL) {
		temp = list->head;
		list->head = temp->next;
		deleteWordNode(temp);
	}
	free(list);
}

void w_fprintList(FILE *fp, WordList *list, void (*fprintData)(FILE *, void *))
{
	WordNode *ptr;
	int count;

	if (!fp || !list || !list->head)
		return;

	ptr = list->head;
	
	if (!fprintData) {
		for (count = 1; ptr != NULL; count++, ptr = ptr->next)
			fprintf(fp, "Node #%d: %s\n", count, ptr->name);
		return;
	}

	for (count = 1; ptr != NULL; count++, ptr = ptr->next) {
		fprintf(fp, "Node #%d:\n\t- name: %s\n\t- data: ", count, ptr->name);
		(*fprintData)(fp, ptr->data);
	}
}
