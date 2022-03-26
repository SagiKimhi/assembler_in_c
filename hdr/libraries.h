#ifndef _LIBRARIES_H
#define _LIBRARIES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define FAILURE -1
#define FILE_EXTENSION_TOKEN		'.'
#define SOURCE_FILE_EXTENSION		".as"
#define PREPROCESSED_FILE_EXTENSION ".am"
#define OBJECT_FILE_EXTENSION		".ob"
#define ENTRY_FILE_EXTENSION		".ent"
#define EXTERN_FILE_EXTENSION		".ext"
#define MAX_LINE_LEN 81			/* maximum length for a line of input, excluding '\0' */
#define MAX_LABEL_LEN 31		/* maximum length for a label, excluding '\0' */
#define MAX_MACRO_LEN 31		/* maximum length for a macro, excluding '\0' */
#define MEM_CELL_SIZE 20		/* the size of a signel cell of memory */
#define MEMSIZE 8192			/* the total number of the RAM's memory cells */
#define MAX_FILE_EXTENSION_LEN 4
#define MAX_OPERATION_WORDS 2
#define MAX_ADDITIONAL_WORDS 4
#define FIRST_MEMORY_ADDRESS 100
#define GENERATE_ENUM(ENUM) ENUM
#define GENERATE_STRING(STRING) #STRING

#if !defined(_STDINT_H) && !defined(__CLANG_STDINT_H)
typedef signed char int8_t;		/* 8 bit signed integer */
typedef unsigned char uint8_t;	/* 8 bit unsigned integer */
typedef signed short int16_t;	/* 16 bit signed integer */
typedef unsigned short uint16_t;/* 16 bit unsigned integer */
typedef signed long int32_t;	/* 32 bit signed integer */
typedef unsigned long uint32_t;	/* 32 bit unsigned integer */
#endif

/* defines a type for a pointer to a function which returns void */
typedef void (*voidOperationPtr)();

#endif

