#include <macro.h>

#define TABLE_SIZE 17
#define MAX_LOAD_FACTOR 0.75
#define REHASH_LOAD_FACTOR 0.25

typedef struct macro_hash_table {
	Macro **table;
	uint32_t tableSize;
	uint32_t mCount;
} MacroTable;

/*	newTable: initates a Macro Table and sets its size to the size given as argument.
	returns 1 upon success, or 0 if the pointer is null or not enough memory could be allocated. */
uint8_t initTable(MacroTable *macroTable, uint32_t size);

/* 	insert: inserts a macro to the macro hash table. returns a pointer
	to the macro upon success or NULL upon failure. */
Macro *insert(MacroTable *macroTable, Macro *macro);

/* search: searches the macro table for a macro with an identical key to the key
	given as argument. returns a pointer to the macro upon success or NULL upon
	failure. */
Macro *search(MacroTable *macroTable, char *key);

/* search: searches the macro table for a macro with an identical key to the key
	given as argument. returns a pointer to the macro upon success or NULL upon
	failure. */
Macro *search(MacroTable *macroTable, char *key);

/* freeTable: deletes a macroTable **AND THE MACROS WITHIN IT** from memory. */
void freeTable(MacroTable *macroTable);