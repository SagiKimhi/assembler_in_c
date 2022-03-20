#ifndef _LABELS_H
#define _LABELS_H
#include <libraries.h>
#include <operations.h>
#include <addressingModes.h>
#include <errors.h>

/* ----------------------------------------------------------------	*
 *			    Constants, Defines, Typedefs, etc.  			    *
 * ----------------------------------------------------------------	*/
/* An X macro for label types generation for macro operations 
 * such as GENERATE_ENUM, GENERATE_STRING, etc. */
#define GENERATE_LABEL_TYPES(OPERATION)\
    OPERATION(UNKNOWN),\
    OPERATION(DATA),\
    OPERATION(STRING),\
    OPERATION(ENTRY),\
    OPERATION(EXTERN),\
    OPERATION(CODE)

#define ADDRESS_BASE 16
#define LOG_2_ADDRESS_BASE 4
#define LABEL_FORMAT "%s:"
#define LABEL_DEFINITION_SUFFIX ':'

typedef struct label Label;

/* An enum of all of the possible label types defined 
 * in the GENERATE_LABEL_TYPES X macro. */
typedef enum LabelTypes {
    GENERATE_LABEL_TYPES(GENERATE_ENUM)
} LabelType;

/* A constant string array which contains strings
 * of all possible label types. The string is generated
 * using the GENERATE_LABEL_TYPES X Macro using
 * GENERATE_STRING operation and therefore names are acceissible
 * using LabelType indices, i.e LabelTypeStr[DATA] == "DATA". */
extern const char *LabelTypeStr[];
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* newLabel: Allocates a new label object in memory and sets its base address
 * and offset according to address argument.
 * Returns a pointer to the new object upon success, or NULL upon failure. 
 *
 * @param address The address in memory which the label points to. 
 * @param type The type of the data which label points to in memory. 
 * @return A pointer to the new allocated Label object. */
Label *newLabel(uint16_t address, LabelType type);

/* deleteLabel: free a label object previously created by newLabel from memory 
 *
 * @param label A pointer to the Label object to be freed from memory. */
void deleteLabel(Label *label);

/* ----------------------------------------------------------------	*
 *								Setters								*
 * ----------------------------------------------------------------	*/
/* setAddress: update label's base address and offset according to address argument 
 *
 * @param label A pointer to the Label object which addresses are to be set.
 * @oaram address The address to be set for label. this address will be split
 * internally by the function to a base address and offset. */
void setLabelAddress(Label *label, uint16_t address);

/* setLabelType: set label's LabelType to the type provided as argument. 
 *
 * @param label A pointer to the Label object which type is to be set.
 * @param type The new type to be set for the label argument. */
void setLabelType(Label *label, LabelType type);

/* ----------------------------------------------------------------	*
 *								Getters								*
 * ----------------------------------------------------------------	*/
/* getAddress: Returns label's address. if label is a NULL object 0 will be returned. 
 *
 * @param label A pointer to a Label object which holds the address to be returned.
 * @return Returns the address which is currently set for label. */
uint16_t getAddress(Label *label);

/* getBaseAddress: Returns the base address of the label.
 * Returns 0 if the argument is a NULL pointer. 
 *
 * @param label A pointer to the label object which holds the address to be returned. 
 * @return Returns the base address of label's address. */
uint16_t getBaseAddress(Label *label);

/* getOffset: Returns the offset of the label from the base address to the label's
 * actual address. Returns 0 if the argument is a NULL pointer. 
 *
 * @param label A pointer to the label object which holds the address to be returned. 
 * @return Returns the offset from the base address to label's address. */
uint16_t getOffset(Label *label);

/* getLabelType: returns the LabelType of the provided Label pointer.
 * if label is a NULL pointer, the LabelType UNKNOWN is returned.
 *
 * @param label The label to get the type of.
 * @return The LabelType of the label pointed to by label. */
LabelType getLabelType(Label *label);

/* ----------------------------------------------------------------	*
 *						Additional Functions						*
 * ----------------------------------------------------------------	*/
/* isValidLabelDefinition: Checks if the expression is a valid label definition.
 * A valid label definition is a valid label tag ending with a LABEL_DEFINITION_SUFFIX.
 * The function holds a flags variable which is instantiated as 0.
 * If an error occures, the flags variable is OR'ed to the apropriate LabelErrorFlags.
 * Upon a valid label definition, 0 is returned. Otherwise, a non 0 value is returned 
 * which can be analyzed by AND operations with the various LabelErrorFlags. 
 * In both cases the actual label name (or part of it if failed due to label length)
 * is saved into the character array argument dest.
 *
 * @param expr A null terminated string of characters to be validated as
 * a label definition expression. 
 * @param dest The destination to which the actual label name/tag should be copied.
 * @return Returns 0 upon success, or a non zero value OR'ed to the appropriate
 * errors of type LabelErrorFlags upon error. */
int isValidLabelDefinition(const char *expr, char dest[MAX_LABEL_LEN+1]);

/* isValidLabelTag: Checks if the expression is a valid label tag/name.
 * The function holds a flags variable which is instantiated as 0.
 * If an error occures, the flags variable is OR'ed to the apropriate LabelErrorFlags.
 * Upon a valid label tag/name, 0 is returned. Otherwise, a non 0 value is returned 
 * which can be analyzed by AND operations with the various LabelErrorFlags.
 *
 * @param expr A null terminated string of characters to be validated as
 * a label definition expression. 
 * @return Returns 0 upon success, or a non zero value OR'ed to the appropriate
 * errors of type LabelErrorFlags upon error. */
int isValidLabelTag(const char *expr);

void printLabel(FILE *stream, Label *label);

#endif
