#include <libraries.h>

#ifndef _TYPES_H
#define _TYPES_H

typedef struct _macro{
	char *key;
	int32_t fStartPos;
	int32_t fEndPos;
	/* add more stuff here later maybe */
} Macro;

typedef struct macro_hash_table {
	Macro **table;
	uint32_t tableSize;
	uint32_t mCount;
} MacroTable;

#endif