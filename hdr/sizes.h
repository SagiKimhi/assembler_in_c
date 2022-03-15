#ifndef _SIZES_H
#define _SIZES_H

#define MAX_LINE_LEN 81			/* maximum length for a line of input, excluding '\0' */
#define MAX_LABEL_LEN 32		/* maximum length for a label, excluding '\0' */
#define MAX_MACRO_LEN 32		/* maximum length for a macro, excluding '\0' */
#define FORMAT_DEFAULT_LEN 50
#define MEM_CELL_SIZE 20		/* the size of a signel cell of memory */
#define MEMSIZE 8192			/* the total number of the RAM's memory cells */
#define DATA_MAX 524287			/* maximum signed 20 bit integer */
#define DATA_MIN 524288			/* minimum signed 20 bit integer */
#define MAX_REGISTER_NUMBER 15	/* maximum register number (rN: r0, r1, ..., rN) */
#define MIN_REGISTER_NUMBER 0	/* minimum register number */
#define MAX_FILE_EXTENSION_LEN 4
#define MAX_OPERATION_WORDS 2
#define MAX_ADDITIONAL_WORDS 4
#define FIRST_MEMORY_ADDRESS 100


#ifndef _STDINT_H
typedef signed char int8_t;		/* 8 bit signed integer */
typedef unsigned char uint8_t;	/* 8 bit unsigned integer */
typedef signed short int16_t;	/* 16 bit signed integer */
typedef unsigned short uint16_t;/* 16 bit unsigned integer */
typedef signed long int32_t;	/* 32 bit signed integer */
typedef unsigned long uint32_t;	/* 32 bit unsigned integer */
#endif

#endif

