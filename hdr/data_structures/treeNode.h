#ifndef _TREE_NODE_H
#define _TREE_NODE_H

#include <libraries.h>

typedef struct treeNode TreeNode;

/* De/Constructors: */
TreeNode *newTreeNode(char *key, void *data);
void deleteTreeNode(TreeNode *node);

/* Setters: */
void setData(TreeNode *node, void *data);
void setLeftChild(TreeNode *node, TreeNode *child);
void setRightChild(TreeNode *node, TreeNode *child);

/* Getters: */
char *getKey(TreeNode *node);
void *getData(TreeNode *node);
TreeNode *getLeftChild(TreeNode *node);
TreeNode *getRightChild(TreeNode *node);
#endif
