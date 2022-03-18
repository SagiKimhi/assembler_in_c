#ifndef _MACRO_H
#define _MACRO_H
#include <libraries.h>
#include <libIO.h>

#define START_OF_MACRO_DEFINITION "macro"
#define END_OF_MACRO_DEFINITION "endm"

typedef struct macro Macro;

/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* newMacro: Allocates space for a new macro struct in memory and instantiates it.
 * Returns a pointer to the new object upon success, or NULL upon failure. 
 *
 * @return Returns a pointer to the new object upon success, or NULL upon failure. */
Macro *newMacro(void);

/* deleteMacro: Frees a macro struct from memory. 
 *
 * @param macro A pointer to the macro struct to be freed from memory. */
void deleteMacro(Macro *macro);

/* ----------------------------------------------------------------	*
 *								Setters								*
 * ----------------------------------------------------------------	*/
/* setStartPosition: Sets position as macro's file position/index for 
 * the begining of the macro's actual contents. 
 *
 * @param macro A pointer to the macro structure which holds the 
 * startPosition to be set.
 * @param position The position/index in the file which defines the macro
 * of the starting point for the macro's actual contents. */
void setStartPosition(Macro *macro, int position);

/* setEndPosition: Sets position as macro's file position/index for 
 * the end of the macro's actual contents. 
 *
 * @param macro A pointer to the macro structure which holds the 
 * endPosition to be set.
 * @param position The position/index in the file which defines the macro
 * of the ending point for the macro's actual contents. */
void setEndPosition(Macro *macro, int position);

/* ----------------------------------------------------------------	*
 *								Getters								*
 * ----------------------------------------------------------------	*/
/* getStartPosition: Returns the starting file index which is currently 
 * set for macro which represents the position in the macro definition
 * file where the actual contents for the macro begin. 
 * A value of 0 is returned if macro is NULL. 
 *
 * @param macro A pointer to the macro structure which holds the startPosition. 
 * @return Returns the startPosition which is currently set for macro,
 * or, if macro is a NULL pointer, 0 will be returned. */
int getStartPosition(Macro *macro);

/* getEndPosition: Returns the ending file index which is currently 
 * set for macro which represents the position in the macro definition
 * file where the actual contents for the macro end. 
 * A value of 0 is returned if macro is NULL. 
 *
 * @param macro A pointer to the macro structure which holds the endPosition. 
 * @return Returns the endPosition which is currently set for macro,
 * or, if macro is a NULL pointer, 0 will be returned. */
int getEndPosition(Macro *macro);

/* ----------------------------------------------------------------	*
 *						Additional Functions						*
 * ----------------------------------------------------------------	*/
/* fprintMacro: Prints the contents of a macro which should exist in
 * readPtr and are pointed to by macro's startPosition and endPosition
 * onto the file pointed to by writePtr. 
 *
 * @param readPtr The file to read the macro's contents from - this has
 * to be the file which defined the macro and it's contents.
 * @param writePtr The output file to be written with the macro's 
 * contents read from readPtr. 
 * @param macro A pointer to the macro structure which hold the starting
 * and ending positions in readPtr of it's contents. */
void fprintMacro(FILE *readPtr, FILE *writePtr, Macro *macro);

#endif

