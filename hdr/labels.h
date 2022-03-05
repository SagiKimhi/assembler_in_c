#ifndef _LABELS_H
#define _LABELS_H

#include <libraries.h>
#include <operations.h>

#define ADDRESS_BASE 16
#define LOG_2_ADDRESS_BASE 4
#define LABEL_FORMAT "%s:"

typedef struct label Label;

typedef enum {
    DATA,
    STRING,
    ENTRY,
    EXTERN,
    CODE
} LabelType;

/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* newLabel: Allocates a new label object in memory and sets its base address
 * and offset according to address argument.
 * Returns a pointer to the new object upon success, or NULL upon failure. */
Label *newLabel(uint16_t address);

/* deleteLabel: free a label object previously created by newLabel from memory */
void deleteLabel(Label *label);

/* ----------------------------------------------------------------	*
 *								Setters								*
 * ----------------------------------------------------------------	*/
/* setAddress: update label's base address and offset according to address */
void setAddress(Label *label, uint16_t address);

/* ----------------------------------------------------------------	*
 *								Getters								*
 * ----------------------------------------------------------------	*/
/* getAddress: Returns label's address */
uint16_t getAddress(Label *label);

/* getBaseAddress: Returns the base address of the label.
 * Returns 0 if the argument is a NULL pointer. */
uint16_t getBaseAddress(Label *label);

/* getOffset: Returns the offset of the label from the base address to the label's
 * actual address. Returns 0 if the argument is a NULL pointer. */
uint16_t getOffset(Label *label);

/* ----------------------------------------------------------------	*
 *						Additional Functions						*
 * ----------------------------------------------------------------	*/
/* isValidLabelName: Checks if the expression is a valid label name.
 * Returns 1 if it is, otherwise returns 0. */
int isValidLabelName(const char *expr);

#endif
