#ifndef _TREE_NODE_H
#define _TREE_NODE_H
#include <libraries.h>
#include <bucket.h>

typedef struct treeNode TreeNode;

/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* newTreeNode: Allocates space for a new treeNode struct in memory and instantiates 
 * it with the key and data provided as arguments using the newBucket function.
 * Returns a pointer to the new object upon success, or NULL upon failure. 
 * 
 * @param key The key to instantiate TreeNode with using newBucket.
 * @param data The data to instantiate TreeNode with using newBucket.
 * @see newBucket(char *key, void *data) 
 * @return Returns a pointer to the new object if successfull or NULL
 * if an error occured during allocation and/or instantiation. */
TreeNode *newTreeNode(char *key, void *data);

/* deleteTreeNode: Free a treeNode struct from memory 
 *
 * @param node The TreeNode to be freed from memory.
 * @param deleteData a void function pointer to a function to free
 * free the data which is pointed to by the TreeNode object, this pointer
 * is later passed and used by deleteBucket.
 * @see deleteBucket(Bucket *bucket, voidOperationPtr deleteData)*/
void deleteTreeNode(TreeNode *node, voidOperationPtr deleteData);

/* ----------------------------------------------------------------	*
 *								Setters								*
 * ----------------------------------------------------------------	*/
/* setData: sets the TreeNode's bucket data pointer to point to data. 
 * 
 * @param node The TreeNode which holds the bucket which will point to data.
 * @param data The data to be pointed to by TreeNode's bucket. 
 * @see setBucketData(Bucket *bucket, void *data) */
void setTreeNodeData(TreeNode *node, void *data);

/* setLeftChild: sets the TreeNode's leftChild pointer to point to child 
 * 
 * @param node The TreeNode who's left child will be pointing to child.
 * @param child The child which node's left child will point to. */
void setLeftChild(TreeNode *node, TreeNode *child);

/* setRightChild: sets the TreeNode's rightChild pointer to point to child 
 * 
 * @param node The TreeNode who's right child will be pointing to child.
 * @param child The child which node's right child will point to. */
void setRightChild(TreeNode *node, TreeNode *child);

/* ----------------------------------------------------------------	*
 *								Getters								*
 * ----------------------------------------------------------------	*/
/* getKey: returns a pointer to the node's key, or NULL if node is NULL. 
 * 
 * @param node The TreeNode object who's key is to be returned.
 * @return Returns a const char pointer to TreeNode's key through getBucketData,
 * Returns NULL if an error occured while attempting to get the key.
 * @see getBucketData(Bucket *bucket).*/
const char *getTreeNodeKey(TreeNode *node);

/* getData: returns a pointer to the data which node points to.
 * Returns NULL if node is NULL. 
 *
 * @param node The TreeNode who's holding the data to be returned.
 * @return Returns a void pointer to the data which is held by node.
 * A NUll pointer will be returned if an error occures or if data is NULL. 
 * @see getBucketData(Bucket *bucket) */
void *getTreeNodeData(TreeNode *node);

/* getLeftChild: Returns a TreeNode pointer to the node's left child
 * which is currently set. Returns NULL if either leftChild or node are NULL.
 *
 * @param node The TreeNode who's left child is to be returned. 
 * @return Returns a TreeNode pointer to the TreeNode which node's left child
 * is pointing to. NULL is returned upon failure or if the left child is NULL. */
TreeNode *getLeftChild(TreeNode *node);

/* getRightChild: Returns a TreeNode pointer to the node's right child which 
 * is currently set. Returns NULL if either rightChild or node are NULL.
 *
 * @param node The TreeNode who's right child is to be returned. 
 * @return Returns a TreeNode pointer to the TreeNode which node's right child
 * is pointing to. NULL is returned upon failure or if the right child is NULL. */
TreeNode *getRightChild(TreeNode *node);

void printTreeNode(FILE *stream, TreeNode *node, voidOperationPtr printData);

#endif
