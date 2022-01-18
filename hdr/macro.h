#ifndef _MACRO_H
#define _MACRO_H
#include <libraries.h>
#include <hashTable.h>

typedef struct _macro{
	int startPos;
	int endPos;
	/* add more stuff here later maybe */
} Macro;

void MacroPreproccessor(FILE *read, FILE *write);
int fscanAndExpandMacros(FILE *read, FILE *write, HashTable *macroTable, int isMacroFlag);
void fexpandMacro(FILE *read, FILE *write, Macro *macro);
void fprintRestOfLine(FILE *readPtr, FILE *writePtr);
void deleteMacro(Macro *macro);
Macro *newMacro();
#endif

