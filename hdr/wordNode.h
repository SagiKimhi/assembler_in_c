#ifndef _WORD_NODE_H
#define _WORD_NODE_H
#include <libraries.h>

typedef struct wordNode {
	char *name;
	void *data;
	struct wordNode *next;
} WordNode;

WordNode *newWordNode(const char *name, void *data);
void deleteWordNode(WordNode *node);
#endif
