#ifndef _SIZES_H
#define SIZES_H


#ifndef MEMSIZE
#define MEMSIZE 8192
#endif

#ifndef CELLSIZE
#define CELLSIZE 20
#define INSTRUCTION_CODE 4
#define OPCODE 16
#define FUNCT 4
#define O_REGISTER 4
#define O_ADDRSS_MODE 2
#define D_REGISTER 4
#define D_ADDRSS_MODE 2
#endif

#ifndef OPERAND
#define OPERAND
#define MIN_OPERANDS 0
#define MAX_OPERANDS 2
#endif

#define MAX_LINE_LEN 82

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed long uint32_t;
typedef unsigned long uint32_t;

#endif