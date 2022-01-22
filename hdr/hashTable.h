#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H
#include <libraries.h>

#ifndef TABLE_SIZE
#define TABLE_SIZE 17
#endif

typedef struct hash_table {
	char **keys;
	void **table;
	size_t tableSize;
	size_t mCount;
} HashTable;

HashTable *newHashTable(int size);
void *insert(HashTable *hashTable, void *ptr, char *key);
void *search(HashTable *hashTable, char *key);
void deleteTable(HashTable *hashTable, int deleteData);

#endif

