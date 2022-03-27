#include <treeNode.h>

/* ----------------------------------------------------------------	*
 *			Constants, Defines, and Structure declarations			*
 * ----------------------------------------------------------------	*/
struct treeNode {
	Bucket *bucket;
	struct treeNode *leftChild;
	struct treeNode *rightChild;
};
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* newTreeNode: Allocates space for a new treeNode struct in memory and instantiates 
 * it with the key and data provided as arguments using the newBucket function.
 * Returns a pointer to the new object upon success, or NULL upon failure. */
TreeNode *newTreeNode(char *key, void *data)
{
	TreeNode *newp;

	if (!(newp = (TreeNode *) malloc(sizeof(TreeNode))))
		return NULL;

	if (!(newp->bucket = newBucket(key, data))) {
		free(newp);
		return NULL;
	}

	setLeftChild(newp, NULL);
	setRightChild(newp, NULL);

	return newp;
}

/* deleteTreeNode: Free a treeNode struct from memory */
void deleteTreeNode(TreeNode *node, voidOperationPtr deleteData)
{
	if (!node)
		return;

	deleteBucket(node->bucket, deleteData);
	free(node);
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *								Setters								*
 * ----------------------------------------------------------------	*/
/* setData: sets the TreeNode's bucket data pointer to point to data. */
void setTreeNodeData(TreeNode *node, void *data)
{
	if (!node)
		return;

	setBucketData(node->bucket, data);
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
const char *getTreeNodeKey(TreeNode *node)
{
	if (!node)
		return NULL;

	return getBucketKey(node->bucket);
}

/* getData: returns a pointer to the data which node points to.
 * Returns NULL if node is NULL. */
void *getTreeNodeData(TreeNode *node)
{
	if (!node)
		return NULL;

	return getBucketData(node->bucket);
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
