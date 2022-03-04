#ifndef _MACRO_H
#define _MACRO_H
#include <libraries.h>
#include <libIO.h>

#define START_OF_MACRO_DEFINITION "macro"
#define END_OF_MACRO_DEFINITION "endm"

typedef struct macro Macro;

/* De/Constructor */
Macro *newMacro();
void deleteMacro(Macro *macro);

/* Setters: */
void setStartPosition(Macro *macro, int position);
void setEndPosition(Macro *macro, int position);

/* Getters: */
int getStartPosition(Macro *macro);
int getEndPosition(Macro *macro);

/* Additional Functions: */
void fprintMacro(FILE *readPtr, FILE *writePtr, Macro *macro);

#endif

