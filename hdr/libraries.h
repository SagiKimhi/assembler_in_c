#ifndef _LIBRARIES_H
#define _LIBRARIES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sizes.h>
#include <limits.h>
#include <encodeBits.h>

#define FAILURE -1
#define CONCATENATE(A,B) A##B
#define GENERATE_ENUM(ENUM) ENUM
#define GENERATE_STRING(STRING) #STRING
#define GENERATE_SCANF_FORMAT(SIZE,FMT_CHAR)\
	GENERATE_STRING(CONCATENATE(CONCATENATE(%,SIZE),FMT_CHAR))

typedef enum BooleanExpression {
	False = 0,
	True = 1
} Bool;

/* defines a type for a pointer to a function which returns void */
typedef void (*voidOperationPtr)();

#endif

