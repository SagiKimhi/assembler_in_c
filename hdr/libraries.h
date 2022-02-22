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
#define GENERATE_ENUM(ENUM) ENUM
#define GENERATE_STRING(STRING) #STRING

typedef enum BooleanExpression {
	False = 0,
	True = 1
} Bool;

#endif

