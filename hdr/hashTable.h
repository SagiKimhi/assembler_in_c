#include <macro.h>

#define TABLE_SIZE 17
#define MAX_LOAD_FACTOR 0.75
#define REHASH_LOAD_FACTOR 0.25

typedef struct macro_hash_table {
	Macro **table;
	uint32_t tableSize;
	uint32_t mCount;
} MacroTable;

