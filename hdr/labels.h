#ifndef _LABELS_H
#define _LABELS_H

#include <libraries.h>
#include <operations.h>

#define ADDRESS_BASE 16
#define LOG_2_ADDRESS_BASE 4

typedef struct label Label;

/* De/Constructor */
Label *newLabel(uint16_t address);
void deleteLabel(Label *label);
/* Setters */
void setAddress(Label *label, uint16_t address);
/* Getters */
uint16_t getAddress(Label *label);
uint16_t getBaseAddress(Label *label);
uint16_t getOffset(Label *label);
/* Additional Functions */
int isValidLabelName(const char *expr);

#endif
