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
	int32_t latestReadPos;

	if (!readPtr || !writePtr || !macro)
		return;

	latestReadPos = ftell(readPtr);
	fseek(readPtr, macro->startPos, SEEK_SET);

	while (ftell(readPtr) < macro->endPos)
		fputc(fgetc(readPtr), writePtr);

	fseek(readPtr, latestReadPos, SEEK_SET);
}
