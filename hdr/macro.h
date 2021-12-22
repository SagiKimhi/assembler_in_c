#include <libraries.h>
#include <types.h>

#ifndef _MACRO_H
#define _MACRO_H

void MacroPreproccessor(FILE *read, FILE *write);
void fexpandMacro(FILE *read, FILE *write, Macro *macro);
uint8_t fscanAndExpandMacros(FILE *read, FILE *write, MacroTable *macroTable, uint8_t isMacroFlag);
void fprintRestOfLine(FILE *rp, FILE *wp);
Macro *newMacro(char *text);
void deleteMacro(Macro *macro);

#endif