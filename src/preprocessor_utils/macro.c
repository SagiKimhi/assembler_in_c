#include <macro.h>

/* ----------------------------------------------------------------	*
 *			Constants, Defines, and Structure declarations			*
 * ----------------------------------------------------------------	*/
struct macro {
	int startPosition;	/* Begining of macro's contents file position/index */
	int endPosition;	/* End of macro's contents file position/index */
};
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* newMacro: Allocates space for a new macro struct in memory and instantiates it.
 * Returns a pointer to the new object upon success, or NULL upon failure. */
Macro *newMacro(void)
{
	Macro *newp = NULL;

	if(!(newp = (Macro *) malloc(sizeof(Macro))))
		return NULL;

	setStartPosition(newp, 0);
	setEndPosition(newp, 0);
	return newp;
}

/* deleteMacro: Frees a macro struct from memory. */
void deleteMacro(Macro *macro)
{
	free(macro);
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *								Setters								*
 * ----------------------------------------------------------------	*/
/* setStartPosition: Sets position as macro's file position/index for 
 * the begining of the macro's actual contents. */
void setStartPosition(Macro *macro, int position)
{
	if (!macro)
		return;

	macro->startPosition = position;
}

/* setEndPosition: Sets position as macro's file position/index for 
 * the end of the macro's actual contents. */
void setEndPosition(Macro *macro, int position)
{
	if (!macro)
		return;

	macro->endPosition = position;
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *								Getters								*
 * ----------------------------------------------------------------	*/
/* getStartPosition: Returns the starting file index which is currently 
 * set for macro which represents the position in the macro definition
 * file where the actual contents for the macro begin. 
 * A value of 0 is returned if macro is NULL. */
int getStartPosition(Macro *macro)
{
	if (!macro)
		return 0;

	return macro->startPosition;
}

/* getEndPosition: Returns the ending file index which is currently 
 * set for macro which represents the position in the macro definition
 * file where the actual contents for the macro end. 
 * A value of 0 is returned if macro is NULL. */
int getEndPosition(Macro *macro)
{
	if (!macro)
		return 0;

	return macro->endPosition;
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *						Additional Functions						*
 * ----------------------------------------------------------------	*/
/* fprintMacro: Prints the contents of a macro which should exist in
 * readPtr and are pointed to by macro's startPosition and endPosition
 * onto the file pointed to by writePtr. */
void fprintMacro(FILE *readPtr, FILE *writePtr, Macro *macro)
{
	int32_t tempReadPosition;

	if (!readPtr || !writePtr || !macro)
		return;

	/* temporarily save the current position in the read file */
	tempReadPosition = ftell(readPtr);
	
	/* print the contents of the macro from readPtr to writePtr */
	copyStream(	readPtr, getStartPosition(macro), getEndPosition(macro),
				writePtr, ftell(writePtr));

	/* seek back to the latest position in the read file */
	fseek(readPtr, tempReadPosition, SEEK_SET);
}
/* ----------------------------------------------------------------	*/
