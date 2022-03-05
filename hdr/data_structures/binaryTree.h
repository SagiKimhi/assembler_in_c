#ifndef _BINARY_TREE_H
#define _BINARY_TREE_H
#include <libraries.h>
#include <treeNode.h>

typedef struct binaryTree Tree;
typedef void (*operation_ptr)();


/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* newTree: Allocates space for a new binaryTree struct in memory and instantiates it.
 * Returns a pointer to the new object upon success, or NULL upon failure. */
Tree *newTree();

/* deleteTree: Free a binaryTree struct from memory including the root
 * and all of it's children. if deleteData is not null, that data stored
 * in the tree nodes will also be deleted using the function it points to. */
void deleteTree(Tree *tree, operation_ptr deleteData);

/* ----------------------------------------------------------------	*
 *								Setters								*
 * ----------------------------------------------------------------	*/
/* addTreeNode: adds a new node to the tree structure. if a node with
 * the same key already exists in the tree a new node will NOT be created. */
void addTreeNode(Tree *tree, char *key, void *data);

/* ----------------------------------------------------------------	*
 *								Getters								*
 * ----------------------------------------------------------------	*/
/* searchTreeNode: Searches a treeNode in the tree structure by key.
 * returns a pointer to the node if such node exists, or NULL otherwise. */
TreeNode *searchTreeNode(Tree *tree, char *key);

#endif
