#include <hashTable.h>

const float MAX_LOAD_FACTOR = 0.75;
const float OPTIMAL_LOAD_FACTOR = 0.25;

/* newHashTable: returns a pointer to a new allocated HashTable or NULL upon failure */
HashTable *newHashTable(int size)
{
	HashTable *newTable;

	if (!(newTable = (HashTable *) malloc(sizeof(HashTable))))
		return NULL;

	if (!(newTable->table = (void **) malloc((sizeof(void *) * size)))) {
		free(newTable);
		return NULL;
	}

	if (!(newTable->keys = (char **) malloc((sizeof(char *) * size)))) {
		free(newTable->table);
		free(newTable);
		return NULL;
	}

	newTable->tableSize = size;
	newTable->mCount = 0;

	return newTable;
}

/*	getNextPrime: returns a positive prime number larger than the argument.
	If the argument is not a prime number the behavior us undefined. */
static size_t getNextPrime(size_t prime)
{
	if (((prime+1)%6) == 0)
		return (((prime+1)/6+1)*6+1);
	return (((prime-1)/6+1)*6-1);
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
	float members = hashTable->mCount;

	if ((members / hashTable->tableSize > MAX_LOAD_FACTOR))
		return 0;

	return 1;
}

static HashTable *rehash(HashTable *hashTable)
{
	HashTable *temp;
	int32_t newPrime, i;
	float members = hashTable->mCount;

	newPrime = hashTable->tableSize;

	while (members/newPrime > OPTIMAL_LOAD_FACTOR)
		newPrime = getNextPrime(newPrime);

	if (!(temp = newHashTable(newPrime)))
		return NULL;

	for (i=0; i<hashTable->tableSize; i++) {
		if (!hashTable->table[i])
			continue;
		insert(temp, hashTable->table[i], hashTable->keys[i]);
		hashTable->table[i] = NULL;
		free(hashTable->keys[i]);
	}

	deleteTable(hashTable, 0);
	return temp;
}

void *insert(HashTable *hashTable, void *ptr, char *key)
{
	uint32_t hkey, startVal, stepVal, fact, index;

	if (!hashTable || !ptr || !key || search(hashTable, key)!=NULL)
		return NULL;

	if (!checkLoadFact(hashTable) && !(hashTable = rehash(hashTable)))
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
	hashTable->table[index] = ptr;

	return hashTable->table[index];
}

void *search(HashTable *hashTable, char *key)
{
	uint32_t hVal, startVal, stepVal, fact, index;

	if (!hashTable || !key || !hashTable->mCount)
		return NULL;

	hVal = hash(key);
	startVal = hash1(hVal, hashTable->tableSize);
	stepVal = hash2(hVal, hashTable->tableSize);
	fact = 1;

	for (index = startVal; hashTable->table[index]!=NULL; ) {
		if (fact > 1 && index == startVal)
			return NULL;

		if (!strcmp(key, hashTable->keys[index]))
			return hashTable->table[index];

		index = doubleHash(startVal, stepVal, fact++, hashTable->tableSize);
	}
	return NULL;
}

/* deleteTable: free the HashTable from memory. */
void deleteTable(HashTable *hashTable, int deleteData)
{
	uint32_t i;
	
	if (!deleteData) {
		free(hashTable->table);
		free(hashTable->keys);
		free(hashTable);
		return;
	}

	for (i=0; i<hashTable->tableSize; i++) {
		if (!hashTable->table[i])
			continue;
		free(hashTable->table[i]);
		free(hashTable->keys[i]);
	}

	free(hashTable->table);
	free(hashTable->keys);
}

