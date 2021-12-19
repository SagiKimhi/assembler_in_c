#include <libraries.h>
#include <macro.h>

#define TABLE_SIZE 17

typedef struct macro_array {
	Macro **column;
	uint16_t arraySize;
} mArray, mTable[TABLE_SIZE];


int8_t addCells(mArray *mArr, uint16_t amount)
{
	Macro **ptr;
	uint16_t i;
	if (mArr->arraySize+amount >= USHRT_MAX)
		return mArr->arraySize;
	ptr = (Macro**) realloc(mArr->column, amount * sizeof(Macro*));
	if (ptr == NULL) {
		/* add handle out of memory error code */
		return mArr->arraySize;
	}
	mArr->column = ptr;
	for (i=mArr->arraySize; i<(mArr->arraySize+=amount); i++)
		mArr->column[i] = NULL;
	return mArr->arraySize;
}

void resetTable(mTable macroTable)
{
	uint16_t row;
	if (!macroTable)
		return;
	for (row=0; row<TABLE_SIZE; row++){
		macroTable[row].column = NULL;
		macroTable[row].arraySize = 0;
	}
}

uint16_t hash(char *key)
{
	uint16_t hash = 5381;
	int32_t c;
	while (c = *key++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

uint16_t hash1(uint16_t hashVal)
{
	return (hashVal%TABLE_SIZE); /* [0,m-1] */
}

/* uint16_t hash2(uint16_t hashVal, uint16_t tabSize)
{
	return ((hashVal%(tabSize-1))+1);	
}

uint16_t doubleHash(uint16_t h1, uint16_t h2, uint16_t fact, uint16_t tabSize)
{
	return ((h1+h2*fact) % tabSize);
} */

Macro *insert(mTable macroTable, Macro *macro)
{
	uint16_t row, col, oldSize;
	if (!macro)
		return NULL;
	row = hash1(hash(macro->key));
	oldSize = macroTable[row].arraySize;
	if ((col = addCells(&macroTable[row], 1)) && col>oldSize)
		return (macroTable[row].column[col] = macro);
	return NULL;
	
}

Macro *search(mTable macroTable, char *key)
{
	uint16_t row, col, oldSize;
	if (!key)
		return NULL;
	row = hash1(hash(key));
	for (col=0; col<macroTable[row].arraySize; col++)
		if (!strcmp(key, macroTable[row].column[col]->key))
			return macroTable[row].column[col];
	return NULL;
}

void freeTable(mTable macroTable)
{
	uint16_t row, col;
	for (row = 0; row<TABLE_SIZE; row++) {
		if (!macroTable[row].column)
			continue;
		for (col=0; col<macroTable[row].arraySize; col++)
			deleteMacro(macroTable[row].column[col]);
		free(macroTable[row].column);
	}
}