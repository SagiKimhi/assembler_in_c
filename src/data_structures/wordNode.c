#include <wordNode.h>

WordNode *newWordNode(const char *name, void *data)
{
	WordNode *newp;

	/* WordNode Struct memory allocation */
	if (!name || !(newp = (WordNode *) malloc(sizeof(WordNode))))
		return NULL;

	/* String dynamic memory allocation */
	if (!(newp->name = (char *) malloc(strlen(name)+1))) {
		free(newp);
		return NULL;
	}

	/* Instantiation */
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
