#include <binaryTree.h>

struct binaryTree {
	TreeNode *root;	
};

/* De/Constructors: */
Tree *newTree()
{
	Tree *newp;

	if (!(newp = (Tree *) malloc(sizeof(Tree))))
		return NULL;

	newp->root = NULL;

	return newp;
}

static void deleteTree_rec(TreeNode *root, void (*deleteData)());
void deleteTree(Tree *tree, void (*deleteData)())
{
	if (!tree)
		return;
	deleteTree_rec(tree->root, deleteData);
	free(tree);
}

/* Setters: */
static TreeNode *addTreeNode_rec(TreeNode *root, char *key, void *data);
void addTreeNode(Tree *tree, char *key, void *data)
{
	if (!tree || !key)
		return;

	tree->root = addTreeNode_rec(tree->root, key, data);
}

/* Getters: */
static TreeNode *searchTreeNode_rec(TreeNode *root, char *key);
TreeNode *searchTreeNode(Tree *tree, char *key)
{
	if (!tree || !key)
		return NULL;

	return searchTreeNode_rec(tree->root, key);
}

static TreeNode *searchTreeNode_rec(TreeNode *root, char *key)
{
	int compare;

	if (!root)
		return NULL;

	if (!(compare = strcmp(getKey(root), key)))
		return root;

	if (compare<0)
		return searchTreeNode_rec(getLeftChild(root), key);

	return searchTreeNode_rec(getRightChild(root), key);
}

static void deleteTree_rec(TreeNode *root, void (*deleteData)())
{
	if (!root)
		return;

	deleteTree_rec(getRightChild(root), deleteData);
	deleteTree_rec(getLeftChild(root), deleteData);
	(*deleteData)(getData(root));
	deleteTreeNode(root);
}

static TreeNode *addTreeNode_rec(TreeNode *root, char *key, void *data)
{
	int compare;

	if (!root)
		return newTreeNode(key, data);

	if (!(compare = strcmp(getKey(root), key)))
		return root;

	if (compare<0)
		setLeftChild(root, addTreeNode_rec(getLeftChild(root), key, data));
	else
		setRightChild(root, addTreeNode_rec(getRightChild(root), key, data));

	return root;
}

