#ifndef _BINARY_TREE_H
#define _BINARY_TREE_H
#include <libraries.h>
#include <treeNode.h>

typedef struct binaryTree Tree;

/* De/Constructors: */
Tree *newTree();
void deleteTree(Tree *tree, void (*deleteData)());

/* Setters: */
void addTreeNode(Tree *tree, char *key, void *data);

/* Getters: */
TreeNode *searchTreeNode(Tree *tree, char *key);

#endif
