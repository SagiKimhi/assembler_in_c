#include <macro.h>

struct macro{
	int startPos;
	int endPos;
	/* add more stuff here later maybe */
};

/* De/Constructor: */
Macro *newMacro()
{
	Macro *newp = NULL;

	if(!(newp = (Macro *) malloc(sizeof(Macro))))
		return NULL;

	setStartPosition(newp, 0);
	setEndPosition(newp, 0);
	return newp;
}

void deleteMacro(Macro *macro)
{
	free(macro);
}

/* Setters: */
void setStartPosition(Macro *macro, int position)
{
	if (!macro)
		return;

	macro->startPos = position;
}

void setEndPosition(Macro *macro, int position)
{
	if (!macro)
		return;

	macro->endPos = position;
}

/* Getters: */
int getStartPosition(Macro *macro)
{
	if (!macro)
		return 0;

	return macro->startPos;
}

int getEndPosition(Macro *macro)
{
	if (!macro)
		return 0;

	return macro->endPos;
}

/* Additional Functions: */
void fprintMacro(FILE *readPtr, FILE *writePtr, Macro *macro)
{
	int32_t tempReadPosition;

	/* FILE Error checking */
	if (!readPtr || !writePtr || !macro)
		return;

	tempReadPosition = ftell(readPtr);
	
	copyStream(readPtr, getStartPosition(macro), getEndPosition(macro), writePtr, ftell(writePtr));
	fseek(readPtr, tempReadPosition, SEEK_SET);
}
