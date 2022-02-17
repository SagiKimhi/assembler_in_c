#ifndef _BINARY_TREE_H
#define _BINARY_TREE_H
#include <libraries.h>

typedef struct treeNode {
	char *key;
	void *data;
	struct treeNode *leftChild;
	struct treeNode *rightChild;
} TreeNode;

typedef struct binaryTree {
	TreeNode *root;	
} Tree;

Tree *newTree();
TreeNode *newTreeNode(char *key, void *data);
TreeNode *addTreeNode(TreeNode *root, char *key, void *data);
void *treeSearch(TreeNode *root, char *key);
void deleteTree(Tree *tree);

#endif
