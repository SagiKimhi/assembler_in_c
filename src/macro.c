#include <macro.h>

Macro *newMacro()
{
	Macro *ptr = NULL;

	if(!(ptr = (Macro *) malloc(sizeof(Macro))))
		return NULL;

	ptr->startPos = -1;
	ptr->endPos = -1;
	return ptr;
}

void deleteMacro(Macro *macro)
{
	free(macro);
}

void fprintMacro(FILE *readPtr, FILE *writePtr, Macro *macro)
{
	int32_t tempReadPosition;

	/* FILE Error checking */
	if (!readPtr || !writePtr || !macro)
		return;

	tempReadPosition = ftell(readPtr);
	
	copyStream(readPtr, macro->startPos, macro->endPos, writePtr, ftell(writePtr));
	fseek(readPtr, tempReadPosition, SEEK_SET);
}
