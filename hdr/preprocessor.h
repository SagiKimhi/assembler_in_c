#ifndef _PREPROCESSOR_H
#define _PREPROCESSOR_H
#include <libraries.h>
#include <binaryTree.h>
#include <hashTable.h>
#include <wordList.h>
#include <macro.h>

#define USE_LIST 0
#define USE_TABLE 0
#define USE_TREE 0

#ifdef __USE_LIST__
#undef USE_LIST
#define USE_LIST 1
#elif defined __USE_TABLE__
#undef USE_TABLE
#define USE_TABLE 1
#elif defined __USE_TREE__
#undef USE_TREE
#define USE_TREE 1
#else
#error "No preprocessor data structure defined. Please define either __USE_LIST__ or __USE_TABLE__ in your makefile."
#endif

void MacroPreproccessor(FILE *read, FILE *write);

#endif
