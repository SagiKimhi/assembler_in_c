#ifndef _LIBRARIES_H
#define _LIBRARIES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <assemblerSyntax.h>

/* Typedefs for declaring variable types with a specific size
 * that is portable and isn't implementation dependent */
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

