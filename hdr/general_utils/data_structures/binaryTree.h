#ifndef _BINARY_TREE_H
#define _BINARY_TREE_H
#include <libraries.h>
#include <treeNode.h>

typedef struct binaryTree Tree;


/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* newTree: Allocates space for a new binaryTree struct in memory and instantiates it.
 * Returns a pointer to the new object upon success, or NULL upon failure. 
 *
 * @return Returns a pointer to a new empty binaryTree data structure object. */
Tree *newTree();

/* deleteTree: Free a binaryTree struct from memory including the root
 * and all of it's children. if deleteData is not null, the data stored
 * in the tree nodes will also be deleted using the function it points to. 
 * The deletion operation for each individual node of the tree is done
 * by calling the deleteTreeNode function for each individual TreeNode. 
 *
 * @param tree The binaryTree data structure to be freed from memory. 
 * @param deleteData An void function pointer to a function which will
 * free the data which is held by the TreeNodes of the binary tree.
 * @see deleteTreeNode(TreeNode *node, voidOperationPtr deleteData) */
void deleteTree(Tree *tree, voidOperationPtr deleteData);

/* ----------------------------------------------------------------	*
 *								Setters								*
 * ----------------------------------------------------------------	*/
/* addTreeNode: adds a new node to the tree structure. if a node with
 * the same key already exists in the tree a new node will NOT be created. 
 *
 * @param tree The binaryTree struct to add the new node to. 
 * @param key The key of the new TreeNode object to be added to the binary tree.
 * @param data The data to be held by new TreeNode object to be added to the binary tree.
 * @see newTreeNode(char *key, void *data) */
void addTreeNode(Tree *tree, char *key, void *data);

/* ----------------------------------------------------------------	*
 *								Getters								*
 * ----------------------------------------------------------------	*/
/* getLeftChild: Returns a TreeNode pointer to the binaryTree's root TreeNode
 * which is currently set. Returns NULL if either the tree or its root are NULL.
 *
 * @param tree A pointer to the Tree object which root is to be returned.
 * @return Returns the root that is currently set for tree as a TreeNode object */
TreeNode *getRoot(Tree *tree);

/* searchTreeNode: Searches a treeNode in the tree structure by key.
 * returns a pointer to the node if such node exists, or NULL otherwise. 
 *
 * @param tree The binaryTree struct which contains the node to be searched for
 * by the specified key. 
 * @param key The key which specifies the TreeNode to be searched for whithin tree. 
 * @return Returns a TreeNode pointer if a TreeNode with the specified key was found,
 * otherwise the function will return a NULL pointer. */
TreeNode *searchTreeNode(Tree *tree, char *key);

#endif
