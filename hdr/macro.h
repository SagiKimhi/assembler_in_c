#ifndef _MACRO_H
#define _MACRO_H
#include <libraries.h>
#include <libIO.h>

#define START_OF_MACRO_DEFINITION "macro"
#define END_OF_MACRO_DEFINITION "endm"

typedef struct _macro{
	int startPos;
	int endPos;
	/* add more stuff here later maybe */
} Macro;

Macro *newMacro();
void deleteMacro(Macro *macro);
void fprintMacro(FILE *readPtr, FILE *writePtr, Macro *macro);
#endif

