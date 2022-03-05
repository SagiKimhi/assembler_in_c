#include <treeNode.h>

struct treeNode {
	char *key;
	void *data;
	struct treeNode *leftChild;
	struct treeNode *rightChild;
};

/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* newTreeNode: Allocates space for a new treeNode struct in memory and 
 * instantiates it by the key and data provided as arguments.
 * Returns a pointer to the new object upon success, or NULL upon failure. */
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

/* deleteTreeNode: Free a treeNode struct from memory */
void deleteTreeNode(TreeNode *node)
{
	if (!node)
		return;

	free(node->key);
	free(node);
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *								Setters								*
 * ----------------------------------------------------------------	*/
/* setData: sets the TreeNode's data pointer to point to data. */
void setData(TreeNode *node, void *data)
{
	if (!node)
		return;

	node->data = data;
}

/* setLeftChild: sets the TreeNode's leftChild pointer to point to child */
void setLeftChild(TreeNode *node, TreeNode *child)
{
	if (!node)
		return;

	node->leftChild = child;
}

/* setRightChild: sets the TreeNode's rightChild pointer to point to child */
void setRightChild(TreeNode *node, TreeNode *child)
{
	if (!node)
		return;

	node->rightChild = child;
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *								Getters								*
 * ----------------------------------------------------------------	*/
/* getKey: returns a pointer to the node's key, or NULL if node is NULL. */
char *getKey(TreeNode *node)
{
	if (!node)
		return NULL;

	return node->key;
}

/* getData: returns a pointer to the data which node points to.
 * Returns NULL if node is NULL. */
void *getData(TreeNode *node)
{
	if (!node)
		return NULL;

	return node->data;
}

/* getLeftChild: Returns a TreeNode pointer to the node's left child
 * which is currently set. Returns NULL if either leftChild or node are NULL.*/
TreeNode *getLeftChild(TreeNode *node)
{
	if (!node)
		return NULL;

	return node->leftChild;
}

/* getRightChild: Returns a TreeNode pointer to the node's right child which 
 * is currently set. Returns NULL if either rightChild or node are NULL.*/
TreeNode *getRightChild(TreeNode *node)
{
	if (!node)
		return NULL;

	return node->rightChild;
}
/* ----------------------------------------------------------------	*/

