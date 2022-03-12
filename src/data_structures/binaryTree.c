#include "libraries.h"
#include "sizes.h"
#include <binaryTree.h>


struct binaryTree {
	TreeNode *root;	
};

static void deleteTree_rec(TreeNode *root, voidOperationPtr deleteData);
static void printTree_rec(FILE *stream, TreeNode *root, voidOperationPtr printData);
static TreeNode *addTreeNode_rec(TreeNode *root, char *key, void *data);
static TreeNode *searchTreeNode_rec(TreeNode *root, char *key);

/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* newTree: Allocates space for a new binaryTree struct in memory and instantiates it.
 * Returns a pointer to the new object upon success, or NULL upon failure. */
Tree *newTree()
{
	Tree *newp;

	if (!(newp = (Tree *) malloc(sizeof(Tree))))
		return NULL;

	newp->root = NULL;

	return newp;
}

/* deleteTree: Free a binaryTree struct from memory including the root
 * and all of it's children. if deleteData is not null, the data stored
 * in the tree nodes will also be deleted using the function it points to. 
 * The deletion operation for each individual node of the tree is done
 * by calling the deleteTreeNode function for each individual TreeNode. */
void deleteTree(Tree *tree, voidOperationPtr deleteData)
{
	if (!tree)
		return;

	deleteTree_rec(tree->root, deleteData);
	free(tree);
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *								Setters								*
 * ----------------------------------------------------------------	*/
/* addTreeNode: adds a new node to the tree structure. if a node with
 * the same key already exists in the tree a new node will NOT be created. */
void addTreeNode(Tree *tree, char *key, void *data)
{
	if (!tree || !key)
		return;

	tree->root = addTreeNode_rec(tree->root, key, data);
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *								Getters								*
 * ----------------------------------------------------------------	*/
/* searchTreeNode: Searches a treeNode in the tree structure by key.
 * returns a pointer to the node if such node exists, or NULL otherwise. */
TreeNode *searchTreeNode(Tree *tree, char *key)
{
	if (!tree || !key)
		return NULL;

	return searchTreeNode_rec(tree->root, key);
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *						Additional Functions						*
 * ----------------------------------------------------------------	*/
void printTree(FILE *stream, Tree *tree, voidOperationPtr printData)
{
	if (!stream || !tree || !tree->root)
		return;

	printTree_rec(stream, tree->root, printData);
}

/* ----------------------------------------------------------------	*
 *							Static Functions						*
 * ----------------------------------------------------------------	*/
/* deleteTree_rec: A private function used by deleteTree to recursively
 * go through the root and children of the tree structure and free
 * them from memory. data will also be free'd if the deleteData is not NULL. */
static void deleteTree_rec(TreeNode *root, voidOperationPtr deleteData)
{
	if (!root)
		return;

	deleteTree_rec(getRightChild(root), deleteData);
	deleteTree_rec(getLeftChild(root), deleteData);
	deleteTreeNode(root, deleteData);
}

/* addTreeNode_rec: A private function used by addTreeNode to recursively
 * go through the root and children of the tree and find an appropriate spot
 * for a new node to be created with key and data. if a node with the same key
 * already exists, nothing new is added to the tree and the function returns. */
static TreeNode *addTreeNode_rec(TreeNode *root, char *key, void *data)
{
	int compare;

	if (!root)
		return newTreeNode(key, data);

	if (!(compare = strcmp(getTreeNodeKey(root), key)))
		return root;

	if (compare<0)
		setLeftChild(root, addTreeNode_rec(getLeftChild(root), key, data));
	else
		setRightChild(root, addTreeNode_rec(getRightChild(root), key, data));

	return root;
}

/* searchTreeNode_rec: A private function used by searchTreeNode to recursively
 * go through the root and children of a tree structure in order to find 
 * the node whose key is identical to the key provided as argument. */
static TreeNode *searchTreeNode_rec(TreeNode *root, char *key)
{
	int compare;

	if (!root)
		return NULL;

	if (!(compare = strcmp(getTreeNodeKey(root), key)))
		return root;

	if (compare<0)
		return searchTreeNode_rec(getLeftChild(root), key);

	return searchTreeNode_rec(getRightChild(root), key);
}

static void printTree_rec(FILE *stream, TreeNode *root, voidOperationPtr printData)
{
	if (!root)
		return;

	printTree_rec(stream, getLeftChild(root), printData);
	printTree_rec(stream, getRightChild(root), printData);

	fprintf(stream, "Key: %-*s\t", 32, getTreeNodeKey(root));

	if (printData!=NULL)
		printData(stream, getTreeNodeData(root));
	else
		fputc('\n', stream);
}
/* ----------------------------------------------------------------	*/

