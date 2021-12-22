#include <libraries.h>
#include <types.h>

#ifndef _MACRO_H
#define _MACRO_H

void fexpandMacro(FILE *fp, Macro *macro);
uint8_t fscanAndExpandMacros(FILE *fp, MacroTable *macroTable, uint8_t isMacroFlag);
void fprintRestOfLine(FILE *fp);
Macro *newMacro(char *text);
void deleteMacro(Macro *macro);

#endif