#include <linkedList.h>

/* 	newElement: Returns a pointer to a new LIST element, or null if not enough
	space could be allocated in memory. */
LIST *newElement(char *text)
{
	LIST *newp = NULL;
	if (!text)
		return NULL;
	if (!(newp = (LIST *)malloc(sizeof(LIST))))
		return NULL;
	if (!(newp->text = (char *)malloc(strlen(text)+1))) {
		free(newp);
		return NULL;
	}
	strcpy(newp->text, text);
	newp->next = NULL;
	return newp;
}

/* deleteElement: removes an element from the list */
LIST *deleteElement(LIST *head, LIST *element)
{
	LIST *ptr;
	if (!head || !element)
		return head;
	if (head==element){
		head = head->next;
		free(element->text);
		free(element);
		return head;
	}
	ptr=head;
	while (ptr && ptr->next!=element) 
		ptr = ptr->next;
	if (!ptr)
		return NULL;
	ptr->next=element->next;
	free(element->text);
	free(element);
	return head;
}

/*	deleteList: deletes the entire list */
void deleteList(LIST *head)
{
	while (deleteElement(head, head))
		;
}

/* addToStart: add an element to the begining of the list. */
LIST *addToStart(LIST *head, LIST *element)
{
	if (!head || !element)
		return head;
	element->next=head;
	return element;
}

/* addToEnd: add an element to the end of the list. */
LIST *addToEnd(LIST *tail, LIST *element)
{
	if (!tail || !element)
		return tail;
	tail->next = element;
	element->next = NULL;
	return element;
}

/* fprintList: prints the text contents of the list's elements onto a file. */
void fprintList(FILE *fp, LIST *head)
{
	LIST *ptr = head;
	if (!fp)
		return;
	while (ptr) {
		fprintf(fp, "%s", ptr->text);
		ptr = ptr->next;
	}
}

