#include <wordNode.h>

WordNode *newWordNode(const char *name, void *data)
{
	WordNode *newp;

	if (!name || !(newp = (WordNode *) malloc(sizeof(WordNode))))
		return NULL;
	if (!(newp->name = (char *) malloc(strlen(name)+1))) {
		free(newp);
		return NULL;
	}

	strcpy(newp->name, name);
	newp->data = data;
	newp->next = NULL;

	return newp;
}

void deleteWordNode(WordNode *node)
{
	if (!node)
		return;
	
	free(node->name);
	free(node->data);
	free(node);
}
