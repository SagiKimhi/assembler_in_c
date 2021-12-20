#include <libraries.h>
#include <sizes.h>

typedef struct node {
	char *text;
	struct node *next;
} LIST;

/*	newElement: Returns a pointer to a new LIST element, or null if not enough
	space could be allocated in memory. */
LIST *newElement(char *text);
/*	deleteElement: removes an element from the list */
LIST *deleteElement(LIST *head, LIST *element);
/*	addToStart: add an element to the begining of the list. */
LIST *addToStart(LIST *head, LIST *element);
/*	addToEnd: add an element to the end of the list. */
LIST *addToEnd(LIST *tail, LIST *element);
/* fprintList: prints the text contents of the list's elements onto a file. */
void fprintList(FILE *fp, LIST *head);
/*	deleteList: deletes the entire list */
void deleteList(LIST *head);