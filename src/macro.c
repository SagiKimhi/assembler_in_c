/*  
*	if (feof(file))
*		return 1;
*	scan the line and look for the word syntax macro
*	if (the macro word was found) {
*		if (the following word already exists in our table)
*			ignore everything until "endm" is reached;
*		else {
*			save the following word into a table (see table description later on...);
*			the name of the macro will be the head of a list;
*			while (the next word is not equal to "endm")
*				set the next character combination as the next element in the list;
*		}
*	}
*	else if (the current word exists in our macro table/array) {
*		go to the next element it points to within the table;
*		while (nextElement != NULL)
*			write it into the text file;
*	}
*	write the line into the output file and return to the begining of the funct;
*	
*	For the purpose of the functionality of this source code we will need:
*	1. A dynamic array of (dynamic arrays/pointers to linked lists)
*	2. A string hashtable implementation (maybe stringTable.c/hashTable.c?)
*	3. A counter to keep track of memory usage.
*
*/
#include <macro.h>

int scanAndExpandMacros(FILE *fp)
{
	char line[MAX_LINE_LEN+1] = {0};
	char *ptr = NULL;
	LIST **macroTable = NULL;
	if (!fp)
		return 0;
	while (fgets(line, MAX_LINE_LEN+1, fp)) {
		if (!(ptr = strstr(line, "macro")))
			return 0;
	}
}

int macroRec(FILE *fp, char *line, uint8_t mFlag)
{
	uint8_t i;
	if (!fp || !line || !fgets(line, MAX_LINE_LEN, fp))
		return 0;
	
}

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
	LIST *ptr, *prev;
	ptr=prev=head;
	while (ptr && ptr!=element) {
		prev = ptr;
		ptr = ptr->next;
	}
	if (!ptr)
		return head;
	if (ptr == head) {
		prev = ptr->next;
		free(ptr->text);
		free(ptr);
		return prev;
	}
	prev->next = ptr->next;
	free(ptr->text);
	free(ptr);
	return head;
}

void deleteList(LIST *head)
{
	if (!head)
		return;
	while (head->next)
		deleteElement(head, head->next);
	deleteElement(head, head);
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