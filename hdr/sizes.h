#ifndef _SIZES_H
#define _SIZES_H
#include <stdio.h>

#define MAX_LINE_LEN 82			/* maximum length for a line of input + '\0' */
#define MAX_LABEL_LEN 32		/* maximum length for a label, excluding '\0' */
#define FORMAT_DEFAULT_LEN 50
#define MAX_OPERATION_LEN 4
#define MEM_CELL_SIZE 20		/* the size of a signel cell of memory */
#define MEMSIZE 8192			/* the total number of the RAM's memory cells */
#define DATA_MAX 524287			/* maximum signed 20 bit integer */
#define DATA_MIN 524288			/* minimum signed 20 bit integer */

#ifndef _STDINT_H
typedef signed char int8_t;		/* 8 bit signed integer */
typedef unsigned char uint8_t;	/* 8 bit unsigned integer */
typedef signed short int16_t;	/* 16 bit signed integer */
typedef unsigned short uint16_t;/* 16 bit unsigned integer */
typedef signed long int32_t;	/* 32 bit signed integer */
typedef unsigned long uint32_t;	/* 32 bit unsigned integer */
#endif

#endif

