#ifndef _BUCKET_H
#define _BUCKET_H
#include <libraries.h>

typedef struct bucket Bucket;

/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* @brief newBucket: Allocates memory for a new bucket struct object. 
 * The bucket's key will be set to key and it's data will point to data.
 *
 * @param key The name/key for this bucket object. 
 * @param data A data object which this bucket object will point to. 
 * @return Returns a pointer to a new allocated bucket object upon success,
 * or a null pointer if any failure occured during the creation process. */
Bucket *newBucket(char *key, void *data);

/* @brief deleteBucket: Frees a Bucket object from memory. the data which bucket points
 * to will also be freed if a non NULL void function pointer is passed as parameter. 
 *
 * @param bucket The bucket object to be freed from memory.
 * @param deleteData A pointer to a void function to free the data which bucket
 * points to from memory. If a null pointer is given as argument, no attempt
 * to free the data pointed to by bucket will be made. */
void deleteBucket(Bucket *bucket, voidOperationPtr deleteData);

/* ----------------------------------------------------------------	*
 *								Setters								*
 * ----------------------------------------------------------------	*/
/* @brief setBucketData: sets the data pointer of bucket to point to data. 
 *
 * @param bucket The Bucket object to be set. 
 * @param data The data object which bucket's data pointer will point to */
void setBucketData(Bucket *bucket, void *data);

/* ----------------------------------------------------------------	*
 *								Getters								*
 * ----------------------------------------------------------------	*/
/* @brief getBucketKey: returns a const char pointer to the bucket's key. 
 *
 * @param bucket The Bucket object which holds they key to be returned. 
 * @return A const char pointer to bucket's key */
const char *getBucketKey(Bucket *bucket);

/* @brief getBucketData: returns the data pointed to by bucket's data pointer. 
 *
 * @param bucket The bucket object which holds the data to be returned.
 * @return Returns a void pointer to the data which bucket's data points to. */
void *getBucketData(Bucket *bucket);

#endif
