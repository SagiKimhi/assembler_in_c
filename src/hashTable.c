#include <hashTable.h>

/*	getNextPrime: returns a positive prime number larger than the argument.
	If the argument is not a prime number the behavior us undefined. */
static uint32_t getNextPrime(uint32_t prime)
{
	if ((prime+1)%6 == 0)
		return (((prime+1)/6+1)*6+1);
	return (((prime-1)/6+1)*6-1);
}

/* 	setTableSize: increases the size of a Macro Table to the size given as argument.
	returns the size of the new table upon success, 0 is returned for a null table,
	or if the new size requested is smaller than the table's current size.
	If not enough space could be allocated, no operation occures and the size of the old
	table is returned. */
int32_t setTableSize(MacroTable *macroTable, uint32_t size)
{
	Macro **ptr;
	uint32_t i, oldSize;
	if (!macroTable || size<macroTable->tableSize)
		return 0;
	ptr = (Macro**) realloc(macroTable->table, size * sizeof(Macro*));
	if (ptr == NULL) {
		/* add handle out of memory error code */
		return macroTable->tableSize;
	}
	macroTable->table = ptr;
	oldSize = macroTable->tableSize;
	macroTable->tableSize += size;
	for (i=oldSize; i<macroTable->tableSize; i++)
		macroTable->table[i] = NULL;
	return macroTable->tableSize;
}

/*	newTable: initates a Macro Table and sets its size to the size given as argument.
	returns 1 upon success, or 0 if the pointer is null or not enough memory could be allocated. */
uint8_t initTable(MacroTable *macroTable, uint32_t size)
{
	uint32_t index;
	if (!macroTable)
		return 0;
	macroTable->table = NULL;
	macroTable->tableSize=0;
	macroTable->mCount=0;
	if (!size)
		return 1;
	if (!setTableSize(macroTable, size))
		return 0;
	return 1;
}

/* hash: returnes a hash value for the given text key argument. */
uint16_t hash(char *key)
{
	uint16_t hash = 5381;
	int32_t c;
	while (c = *key++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

/*	hash1: default function for generating an index in a table from a given hash value. */
uint32_t hash1(uint32_t hashVal, uint32_t tableSize)
{
	return (hashVal%tableSize); /* [0,m-1] */
}

/*	hash2: second function for hash table index generation from a hash value
	to be used only for double hashing. this alone will not return a valid index for a key. */
static uint32_t hash2(uint32_t hashVal, uint32_t tableSize)
{
	return ((hashVal%(tableSize-1))+1); /* [1,m-1] */
}

/*	doubleHash: a function used for double hashing to generate a new index in the case of encountering
	a collision with the index returned by hash1 function. */
static uint32_t doubleHash(uint32_t h1, uint32_t h2, uint32_t fact, uint32_t tableSize)
{
	return ((h1+h2*fact) % tableSize);
}

/* 	checkLoadFact: returns 1 if the table's load factor is below the defined MAX_LOAD_FACTOR,
	returns 0 otherwise. Returns -1 if the MacroTable pointer argument is NULL. */
int32_t checkLoadFact(MacroTable *macroTable)
{
	if (!macroTable)
		return -1;
	if (((float)macroTable->mCount)/macroTable->tableSize > MAX_LOAD_FACTOR)
		return 0;
	return 1;
}

/* rehash: rehashes the macroTable onto a new larger MacroTable, maintaining a maximum
	load factor of the defined REHASH_LOAD_FACTOR. returns a pointer to the new table upon success,
	or NULL pointer if the function failed for any reason.  */
Macro *rehash(MacroTable *macroTable)
{
	MacroTable temp;
	uint32_t oldSize, newPrime, i;
	float count;
	newPrime = macroTable->tableSize;
	while ((count = macroTable->mCount)/newPrime > REHASH_LOAD_FACTOR)
		newPrime = getNextPrime(newPrime);
	if (!initTable(&temp, newPrime))
		return NULL;
	for (i=0; i<macroTable->tableSize; i++)
		insert(&temp, macroTable->table[i]);
	freeTable(macroTable);
	macroTable = &temp;
	return macroTable;
}

/* 	insert: inserts a macro to the macro hash table. returns a pointer
	to the macro upon success or NULL upon failure. */
Macro *insert(MacroTable *macroTable, Macro *macro)
{
	uint32_t hkey, startVal, stepVal, fact, index, oldSize, primeNum;
	if (!macroTable || !macro || search(macroTable, macro->key))
		return NULL;
	if (!checkLoadFact(macroTable))
		if(!rehash(macroTable) && macroTable->mCount>=macroTable->tableSize)
			return NULL;
	hkey = hash(macro->key);
	index = startVal = hash1(hkey, macroTable->tableSize);
	stepVal = hash2(hkey, macroTable->tableSize);
	fact = 1;
	while (macroTable->table[index])
		index = doubleHash(startVal, stepVal, fact++, macroTable->tableSize);
	macroTable->mCount++;
	return (macroTable->table[index] = macro);
}

/* search: searches the macro table for a macro with an identical key to the key
	given as argument. returns a pointer to the macro upon success or NULL upon
	failure. */
Macro *search(MacroTable *macroTable, char *key)
{
	uint32_t hVal, startVal, stepVal, fact, index;
	if (!macroTable || !key || !macroTable->mCount)
		return NULL;
	hVal = hash(key);
	index = startVal = hash1(hVal, macroTable->tableSize);
	stepVal = hash2(hVal, macroTable->tableSize);
	fact = 1;
	while (macroTable->table[index]) {
		if (fact>1 && index == startVal)
			return NULL;
		if (!strcmp(key, macroTable->table[index]->key))
			return macroTable->table[index];
		index = doubleHash(startVal, stepVal, fact++, macroTable->tableSize);
	}
	return NULL;
}

/* freeTable: deletes a macroTable **AND THE MACROS WITHIN IT** from memory. */
void freeTable(MacroTable *macroTable)
{
	uint16_t index;
	for (index = 0; index<macroTable->tableSize; index++) {
		if (!macroTable->table[index])
			continue;
		deleteMacro(macroTable->table[index]);
	}
	free(macroTable->table);
	initTable(macroTable, 0);
}