#include <data_structures/bucket.h>

Bucket *newBucket(char *key, void *data)
{
	Bucket *newp;

	if (!key)
		return NULL;

	if (!(newp = (Bucket *) malloc(sizeof(Bucket))))
		return NULL;
	
	if (!(newp->key = (char *) malloc(strlen(key)+1))) {
		free(newp);
		return NULL;
	}

	strcpy(newp->key, key);
	newp->data = data;

	return newp;
}

void deleteBucket(Bucket *bucket, void (*deleteData)())
{
	if (!bucket)
		return;
	
	if (!deleteData) {
		free(bucket->key);
		free(bucket);
		return;
	}

	(*deleteData)(bucket->data);
	free(bucket->key);
	free(bucket);
}
