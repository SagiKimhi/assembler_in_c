#include <treeNode.h>

struct treeNode {
	char *key;
	void *data;
	struct treeNode *leftChild;
	struct treeNode *rightChild;
};

/* De/Constructors: */
TreeNode *newTreeNode(char *key, void *data)
{
	TreeNode *newp;

	if (!key)
		return NULL;

	if (!(newp = (TreeNode *) malloc(sizeof(TreeNode))))
		return NULL;

	if (!(newp->key = (char *) malloc(strlen(key)+1))) {
		free(newp);
		return NULL;
	}

	strcpy(newp->key, key);
	setData(newp, data);
	setLeftChild(newp, NULL);
	setRightChild(newp, NULL);

	return newp;
}

void deleteTreeNode(TreeNode *node)
{
	if (!node)
		return;

	free(node->key);
	free(node);
}

/* Setters: */
void setData(TreeNode *node, void *data)
{
	if (!node)
		return;

	node->data = data;
}
void setLeftChild(TreeNode *node, TreeNode *child)
{
	if (!node)
		return;

	node->leftChild = child;
}

void setRightChild(TreeNode *node, TreeNode *child)
{
	if (!node)
		return;

	node->rightChild = child;
}

/* Getters: */
char *getKey(TreeNode *node)
{
	if (!node)
		return NULL;

	return node->key;
}

void *getData(TreeNode *node)
{
	if (!node)
		return NULL;

	return node->data;
}

TreeNode *getLeftChild(TreeNode *node)
{
	if (!node)
		return NULL;

	return node->leftChild;
}

TreeNode *getRightChild(TreeNode *node)
{
	if (!node)
		return NULL;

	return node->rightChild;
}
