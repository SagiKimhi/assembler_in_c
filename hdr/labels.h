#ifndef _LABELS_H
#define _LABELS_H
#include <libraries.h>
#include <operations.h>
#include <addressingModes.h>

#define ADDRESS_BASE 16
#define LOG_2_ADDRESS_BASE 4
#define LABEL_FORMAT "%s:"
#define LABEL_DEFINITION_SUFFIX ':'
#define GENERATE_LABEL_TYPE(OPERATION)\
    OPERATION(UNKNOWN),\
    OPERATION(DATA),\
    OPERATION(STRING),\
    OPERATION(ENTRY),\
    OPERATION(EXTERN),\
    OPERATION(CODE)

typedef struct label Label;

typedef enum LabelTypes {
    GENERATE_LABEL_TYPE(GENERATE_ENUM)
} LabelType;

extern const char *LabelTypeStr[];

/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* newLabel: Allocates a new label object in memory and sets its base address
 * and offset according to address argument.
 * Returns a pointer to the new object upon success, or NULL upon failure. */
Label *newLabel(uint16_t address, LabelType type);

/* deleteLabel: free a label object previously created by newLabel from memory */
void deleteLabel(Label *label);

/* ----------------------------------------------------------------	*
 *								Setters								*
 * ----------------------------------------------------------------	*/
/* setAddress: update label's base address and offset according to address */
void setLabelAddress(Label *label, uint16_t address);

void setLabelType(Label *label, LabelType type);

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

/** @brief Returns the tyupe of the label as LabelType(Data, String, Entry, Extern, Code).
*   @param label the label to get the type of.
*   @return the label type.
*/
LabelType getLabelType(Label *label);

/* ----------------------------------------------------------------	*
 *						Additional Functions						*
 * ----------------------------------------------------------------	*/
/* isValidLabelDefinition: Checks if the expression is a valid label name.
 * Returns 1 if it is, otherwise returns 0. */
int isValidLabelDefinition(const char *expr, char dest[MAX_LABEL_LEN+1]);

int isValidLabelTag(const char *expr);

void printLabel(FILE *stream, Label *label);
#endif
