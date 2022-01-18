#include <hashTable.h>

/*	getNextPrime: returns a positive prime number larger than the argument.
	If the argument is not a prime number the behavior us undefined. */
static size_t getNextPrime(size_t prime)
{
	if ((prime+1)%6 == 0)
		return (((prime+1)/6+1)*6+1);
	return (((prime-1)/6+1)*6-1);
}

/* 	setTableSize: increases the size of a void Table to the size given as argument.
	returns the size of the new table upon success, 0 is returned for a null table,
	or if the new size requested is smaller than the table's current size.
	If not enough space could be allocated, no operation occures and the size of the old
	table is returned. */
static size_t setTableSize(HashTable *hashTable, size_t size)
{
	void **tPtr; /* table pointer */
	char **kPtr; /* keys pointer */
	size_t i, oldSize;

	if (!hashTable || size <= hashTable->tableSize)
		return 0;

	tPtr = (void **) realloc(hashTable->table, size * sizeof(void *));
	kPtr = (char **) realloc(hashTable->keys, size * sizeof(char *));
	
	if (!tPtr || !kPtr)
		return 0;

	hashTable->table = tPtr;
	hashTable->keys = kPtr;
	oldSize = hashTable->tableSize;
	hashTable->tableSize = size;

	for (i=oldSize; i < hashTable->tableSize; i++) {
		hashTable->table[i] = NULL;
		hashTable->keys[i] = NULL;
	}

	return hashTable->tableSize;
}

HashTable *newHashTable()
{
	HashTable *newTable = (HashTable *) malloc(sizeof(HashTable));
	if (!newTable)
		return NULL;

	newTable->table = NULL;
	newTable->keys = NULL;
	newTable->tableSize = 0;
	newTable->mCount = 0;

	return newTable;
}

/*	newTable: initates a void Table and sets its size to the size given as argument.
	returns 1 upon success, or 0 if the pointer is null or not enough memory could be allocated. */
int initTable(HashTable *hashTable, size_t size)
{
	if (hashTable != NULL)
		deleteTable(hashTable);

	if (!(hashTable = newHashTable()))
			return 0;

	if (!setTableSize(hashTable, size))
		return 0;

	return 1;
}

/* hash: returnes a hash value for the given text key argument. */
static uint32_t hash(char *key)
{
	uint32_t hash = 5381;
	int32_t c;
	while ((c = *key++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

/*	hash1: default function for generating an index in a table from a given hash value. */
static uint32_t hash1(uint32_t hashVal, size_t tableSize)
{
	return (hashVal%tableSize); /* [0,m-1] */
}

/*	hash2: second function for hash table index generation from a hash value
	to be used only for double hashing. this alone will not return a valid index for a key. */
static uint32_t hash2(uint32_t hashVal, size_t tableSize)
{
	return ((hashVal%(tableSize-1))+1); /* [1,m-1] */
}

/*	doubleHash: a function used for double hashing to generate a new index in the case of encountering
	a collision with the index returned by hash1 function. */
static uint32_t doubleHash(uint32_t h1, uint32_t h2, uint32_t fact, size_t tableSize)
{
	return ((h1+h2*fact) % tableSize);
}

/* 	checkLoadFact: returns 1 if the table's load factor is below the defined MAX_LOAD_FACTOR,
	returns 0 otherwise. Returns -1 if the HashTable pointer argument is NULL. */
static int32_t checkLoadFact(HashTable *hashTable)
{
	if (!hashTable)
		return 0;

	if (((float)hashTable->mCount) / hashTable->tableSize > MAX_LOAD_FACTOR)
		return 0;

	return 1;
}

/* rehash: rehashes the hashTable onto a new larger HashTable, maintaining a maximum
	load factor of the defined REHASH_LOAD_FACTOR. returns a pointer to the new table upon success,
	or NULL pointer if the function failed for any reason.  */
static HashTable *rehash(HashTable *hashTable)
{
	HashTable *temp = (HashTable *) malloc(sizeof(HashTable));
	size_t newPrime, index;

	if (!temp)
		return NULL;

	newPrime = hashTable->tableSize;
	while (((float)hashTable->mCount)/newPrime > REHASH_LOAD_FACTOR)
		newPrime = getNextPrime(newPrime);

	if (!initTable(temp, newPrime))
		return NULL;

	for (index=0; index<hashTable->tableSize; index++)
		insert(temp, hashTable->table[index], hashTable->keys[index]);

	deleteTable(hashTable);
	hashTable = temp;
	return hashTable;
}

/* 	insert: inserts a macro to the macro hash table. returns a pointer
	to the macro upon success or NULL upon failure. */
void *insert(HashTable *hashTable, void *ptr, char *key)
{
	uint32_t hkey, startVal, stepVal, fact, index;

	if (!hashTable || !ptr || !key || (search(hashTable, key)!=NULL))
		return NULL;

	if (!checkLoadFact(hashTable))
		if(!rehash(hashTable) && hashTable->mCount>=hashTable->tableSize)
			return NULL;

	hkey = hash(key);
	index = startVal = hash1(hkey, hashTable->tableSize);
	stepVal = hash2(hkey, hashTable->tableSize);
	fact = 1;

	while (hashTable->keys[index] != NULL)
		index = doubleHash(startVal, stepVal, fact++, hashTable->tableSize);

	if (!(hashTable->keys[index] = malloc(strlen(key)+1)))
		return NULL;

	hashTable->mCount++;
	strcpy(hashTable->keys[index], key);
	return (hashTable->table[index] = ptr);
}

/* search: searches the macro table for a macro with an identical key to the key
	given as argument. returns a pointer to the macro upon success or NULL upon
	failure. */
void *search(HashTable *hashTable, char *key)
{
	uint32_t hVal, startVal, stepVal, fact, index;

	if (!hashTable || !key || !hashTable->mCount)
		return NULL;

	hVal = hash(key);
	index = startVal = hash1(hVal, hashTable->tableSize);
	stepVal = hash2(hVal, hashTable->tableSize);
	fact = 1;

	while (hashTable->table[index]) {
		if (fact > 1 && index == startVal)
			return NULL;
		if (!strcmp(key, hashTable->keys[index]))
			return hashTable->table[index];

		index = doubleHash(startVal, stepVal, fact++, hashTable->tableSize);
	}
	return NULL;
}

/* deleteTable: free the HashTable from memory. */
void deleteTable(HashTable *hashTable)
{
	uint32_t index;
	for (index = 0; index < hashTable->tableSize; index++) {
		if (!hashTable->table[index])
			continue;
		free(hashTable->table[index]);
		free(hashTable->keys[index]);
	}
	free(hashTable->table);
	free(hashTable->keys);
	free(hashTable);
}

