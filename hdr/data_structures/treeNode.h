#ifndef _TREE_NODE_H
#define _TREE_NODE_H

#include <libraries.h>

typedef struct treeNode TreeNode;

/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* newTreeNode: Allocates space for a new treeNode struct in memory and 
 * instantiates it by the key and data provided as arguments.
 * Returns a pointer to the new object upon success, or NULL upon failure. */
TreeNode *newTreeNode(char *key, void *data);

/* deleteTreeNode: Free a treeNode struct from memory */
void deleteTreeNode(TreeNode *node);

/* ----------------------------------------------------------------	*
 *								Setters								*
 * ----------------------------------------------------------------	*/
/* setData: sets the TreeNode's data pointer to point to data. */
void setData(TreeNode *node, void *data);

/* setLeftChild: sets the TreeNode's leftChild pointer to point to child */
void setLeftChild(TreeNode *node, TreeNode *child);

/* setRightChild: sets the TreeNode's rightChild pointer to point to child */
void setRightChild(TreeNode *node, TreeNode *child);

/* ----------------------------------------------------------------	*
 *								Getters								*
 * ----------------------------------------------------------------	*/
/* getKey: returns a pointer to the node's key, or NULL if node is NULL. */
char *getKey(TreeNode *node);

/* getData: returns a pointer to the data which node points to.
 * Returns NULL if node is NULL. */
void *getData(TreeNode *node);

/* getLeftChild: Returns a TreeNode pointer to the node's left child
 * which is currently set. Returns NULL if either leftChild or node are NULL.*/
TreeNode *getLeftChild(TreeNode *node);

/* getRightChild: Returns a TreeNode pointer to the node's right child which 
 * is currently set. Returns NULL if either rightChild or node are NULL.*/
TreeNode *getRightChild(TreeNode *node);

#endif
