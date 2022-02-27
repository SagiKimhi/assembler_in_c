#ifndef _PREPROCESSOR_H
#define _PREPROCESSOR_H
#include <libIO.h>
#include <macro.h>
#include <libraries.h>
#include <binaryTree.h>

#define START_OF_MACRO_DEFINITION "macro"
#define END_OF_MACRO_DEFINITION "endm"

void macroPreprocessor(FILE *read, FILE *write);

#endif
