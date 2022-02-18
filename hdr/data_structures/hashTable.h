#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H
#include <libraries.h>
#include <bucket.h>

#ifndef TABLE_SIZE
#define TABLE_SIZE 17
#endif

typedef struct hash_table {
	Bucket **bucket;
	size_t mCount;
	size_t tableSize;
} HashTable;

HashTable *newHashTable(int size);
void *search(HashTable *hashTable, char *key);
void *insert(HashTable *hashTable, char *key, void *data);
void deleteTable(HashTable *hashTable, void (*deleteData)());

#endif

