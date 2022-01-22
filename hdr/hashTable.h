#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H
#include <libraries.h>

#define TABLE_SIZE 17
#define MAX_LOAD_FACTOR 0.75
#define REHASH_LOAD_FACTOR 0.25

typedef struct hash_table {
	void **table;
	char **keys;
	size_t tableSize;
	size_t mCount;
} HashTable;

HashTable *newHashTable(size_t size);
/*	newTable: initates a Macro Table and sets its size to the size given as argument.
	returns 1 upon success, or 0 if the pointer is null or not enough memory could be allocated. */
int initTable(HashTable *macroTable, size_t size);

/* 	insert: inserts a macro to the macro hash table. returns a pointer
	to the macro upon success or NULL upon failure. */
void *insert(HashTable *hashTable, void *ptr, char *key);

/* search: searches the macro table for a macro with an identical key to the key
	given as argument. returns a pointer to the macro upon success or NULL upon
	failure. */
void *search(HashTable *hashTable, char *key);

/* deleteTable: deletes a macroTable **AND THE MACROS WITHIN IT** from memory. */
void deleteTable(HashTable *hashTable);

#endif

