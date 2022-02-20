#ifndef _PREPROCESSOR_H
#define _PREPROCESSOR_H
#include <macro.h>
#include <libraries.h>
#include <binaryTree.h>
#include <handleInput.h>

#define START_OF_MACRO_DEFINITION "macro"
#define END_OF_MACRO_DEFINITION "endm"

void macroPreprocessor(FILE *read, FILE *write);

#endif
