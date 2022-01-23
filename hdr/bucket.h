#ifndef _BUCKET_H
#define _BUCKET_H
#include <libraries.h>

typedef struct bucket {
	char *key;
	void *data;
} Bucket;

Bucket *newBucket(char *key, void *data);
void deleteBucket(Bucket *bucket, void (*deleteData)());
#endif
