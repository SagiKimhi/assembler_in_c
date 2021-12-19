#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sizes.h>

typedef struct element {
	char *text;
	struct element *next;
} LIST;

typedef struct _macro{
	uint8_t *key;
	fpos_t fPosition;
	/* add more stuff here later maybe */
} Macro;