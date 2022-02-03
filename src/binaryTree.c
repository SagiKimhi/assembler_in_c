#include <binaryTree.h>

Tree *newTree()
{
	Tree *newp;

	if (!(newp = (Tree *) malloc(sizeof(Tree))))
		return NULL;

	newp->root = NULL;

	return newp;
}

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
	newp->data = data;
	newp->leftChild = NULL;
	newp->rightChild = NULL;

	return newp;
}

TreeNode *addTreeNode(TreeNode *root, char *key, void *data)
{
	int compare;

	if (!key)
		return NULL;

	if (!root)
		return (root = newTreeNode(key, data));

	if (!(compare = strcmp(root->key, key)))
		return root;

	if (compare<0)
		root->leftChild = addTreeNode(root->leftChild, key, data);
	else
		root->rightChild = addTreeNode(root->rightChild, key, data);

	return root;
}

void *treeSearch(TreeNode *root, char *key)
{
	int compare;

	if (!root || !key)
		return NULL;

	if (!(compare = strcmp(root->key, key)))
		return root->data;

	if (compare<0)
		return treeSearch(root->leftChild, key);

	return treeSearch(root->rightChild, key);
}

static void *deleteTree_rec(TreeNode *root)
{
	if (root->leftChild != NULL)
		root->leftChild = deleteTree_rec(root->leftChild);

	if (root->rightChild != NULL)
		root->rightChild = deleteTree_rec(root->rightChild);

	free(root->data);
	free(root->key);
	free(root);

	return NULL;
}

void deleteTree(Tree *tree)
{
	if (!tree)
		return;
	deleteTree_rec(tree->root);
	free(tree);
}
