#include <bucket.h>

/* ----------------------------------------------------------------	*
 *			Constants, Defines, and Structure declarations			*
 * ----------------------------------------------------------------	*/
struct bucket {
	char *key;
	void *data;
};
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *						Static Function Prototypes					*
 * ----------------------------------------------------------------	*/
/* setBucketKey: A private function used by the constructor to allocate
 * memory and store a copy of key in the bucket's key variable.
 * Returns a pointer to bucket's key on success, or NULL upon failure. */
static char *setBucketKey(Bucket *bucket, char *key);
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* newBucket: Allocates memory for a new bucket struct object. 
 * The bucket's key will be set to key and it's data will point to data. */
Bucket *newBucket(char *key, void *data)
{
	Bucket *newp;

	if (!key)
		return NULL;

	if (!(newp = (Bucket *) malloc(sizeof(Bucket))))
		return NULL;
	
	if (!setBucketKey(newp, key)) {
		free(newp);
		return NULL;
	}

	setBucketData(newp, data);
	return newp;
}

/* deleteBucket: Frees a Bucket object from memory. the data which bucket points
 * to will also be freed if a non NULL void function pointer is passed as parameter. */
void deleteBucket(Bucket *bucket, voidOperationPtr deleteData)
{
	if (!bucket)
		return;
	
	if (deleteData!=NULL && bucket->data!=NULL)
		deleteData(bucket->data);

	free(bucket->key);
	free(bucket);
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *								Setters								*
 * ----------------------------------------------------------------	*/
/* setBucketData: sets the data pointer of bucket to point to data. */
void setBucketData(Bucket *bucket, void *data)
{
	if (!bucket)
		return;

	bucket->data = data;
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *								Getters								*
 * ----------------------------------------------------------------	*/
/* getBucketKey: returns a const char pointer to the bucket's key. */
const char *getBucketKey(Bucket *bucket)
{
	if (!bucket)
		return NULL;

	return bucket->key;
}

/* getBucketData: returns the data pointed to by bucket's data pointer. */
void *getBucketData(Bucket *bucket)
{
	if (!bucket)
		return NULL;

	return bucket->data;
}

/* ----------------------------------------------------------------	*/

void printBucket(FILE *stream, Bucket *bucket, voidOperationPtr printData)
{
	if (!bucket)
		return;

	fprintf(stream, "Key: %-*s\t", 32, getBucketKey(bucket));

	if (printData!=NULL)
		printData(stream, getBucketData(bucket));
	else
		fputc('\n', stream);
}

/* ----------------------------------------------------------------	*
 *							Static Functions						*
 * ----------------------------------------------------------------	*/
/* setBucketKey: A private function used by the constructor to allocate
 * memory and store a copy of key in the bucket's key variable.
 * Returns a pointer to bucket's key on success, or NULL upon failure. */
static char *setBucketKey(Bucket *bucket, char *key)
{
	if (!key)
		return NULL;
	
	bucket->key = (char *) malloc(strlen(key)+1);
	strcpy(bucket->key, key);

	return bucket->key;
}
/* ----------------------------------------------------------------	*/
