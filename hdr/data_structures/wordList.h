#ifndef _WORD_LIST_H
#define _WORD_LIST_H
#include <libraries.h>
#include <wordNode.h>

typedef struct wordList {
	WordNode *head, *tail;
} WordList;

WordList *newWordList();
WordList *w_addToFront(WordList *list, const char *name, void *data);
WordList *w_addToEnd(WordList *list, const char *name, void *data);
void *w_searchList(WordList *list, const char *name);
void w_fprintList(FILE *fp, WordList *list, void (*fprintData)(FILE *, void *));
void w_deleteListNode(WordList *list, char *name);
void w_deleteList(WordList *list);
#endif
