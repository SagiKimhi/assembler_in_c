#include <labels.h>

/* ----------------------------------------------------------------	*
 *			Constants, Defines, and Structure declarations			*
 * ----------------------------------------------------------------	*/
struct label {
	uint16_t address;
	LabelType type;
};

const char *LabelTypeStr[] = {
	GENERATE_LABEL_TYPES(GENERATE_STRING)
};
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* newLabel: Allocates a new label object in memory and sets its base address
 * and offset according to address argument, and type according to the type argument.
 * Returns a pointer to the new object upon success, or NULL upon failure. */
Label *newLabel(uint16_t address, LabelType type)
{
	Label *newp = (Label *) malloc(sizeof(Label));

	if (!newp)
		return NULL;

	setLabelAddress(newp, address);
	setLabelType(newp, type);

	return newp;
}

/* deleteLabel: free a label object previously created by newLabel from memory */
void deleteLabel(Label *label)
{
	free(label);
}
/* ----------------------------------------------------------------	*/


/* ----------------------------------------------------------------	*
 *								Setters								*
 * ----------------------------------------------------------------	*/
/* setAddress: update label's base address and offset according to address argument */
void setLabelAddress(Label *label, uint16_t address)
{
	if (!label)
		return;

	label->address = address;
}

/* setLabelType: set label's LabelType to the type provided as argument. */
void setLabelType(Label *label, LabelType type)
{
	if (!label)
		return;

	label->type = type;
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *								Getters								*
 * ----------------------------------------------------------------	*/
/* getAddress: Returns label's address. if label is a NULL object 0 will be returned. */
uint16_t getAddress(Label *label)
{
	if (!label)
		return 0;

	return label->address;
}

/* getBaseAddress: Returns the base address of the label.
 * Returns 0 if the argument is a NULL pointer. */
uint16_t getBaseAddress(Label *label)
{
	return (ADDRESS_BASE * (getAddress(label) / ADDRESS_BASE));
}

/* getOffset: Returns the offset of the label from the base address to the label's
 * actual address. Returns 0 if the argument is a NULL pointer. */
uint16_t getOffset(Label *label)
{
	return (getAddress(label) % ADDRESS_BASE);
}

/* getLabelType: returns the LabelType of the provided Label pointer.
 * if label is a NULL pointer, the LabelType UNKNOWN is returned. */
LabelType getLabelType(Label *label)
{
	if (!label)
		return UNKNOWN;

	return label->type;
}
/* ----------------------------------------------------------------	*/


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
 * is saved into the character array argument dest. */
int isValidLabelDefinition(const char *expr, char dest[MAX_LABEL_LEN+1])
{
	int flags = 0, i = 0;

	if (!expr || !(*expr))
		return FAILURE;

	while (*expr && *expr!=LABEL_DEFINITION_SUFFIX) {
		if (i<MAX_LABEL_LEN)
			dest[i++] = *expr++;
		else
			flags |= INVALID_LABEL_LEN;
	}
	
	dest[i] = '\0';

	if (*expr!=LABEL_DEFINITION_SUFFIX || *(expr+1)!='\0')
		flags |= MISSING_LABEL_DEFINITION_SUFFIX;

	flags |= isValidLabelTag(dest);

	return flags;
}

/* isValidLabelTag: Checks if the expression is a valid label tag/name.
 * The function holds a flags variable which is instantiated as 0.
 * If an error occures, the flags variable is OR'ed to the apropriate LabelErrorFlags.
 * Upon a valid label tag/name, 0 is returned. Otherwise, a non 0 value is returned 
 * which can be analyzed by AND operations with the various LabelErrorFlags. */
int isValidLabelTag(const char *expr)
{
	int flags = 0;
	const char *ptr = expr;

	if (!expr || !(*expr))
		return FAILURE;

	if (!isalpha(*ptr))
		flags |= INVALID_LABEL_SYNTAX;

	while (isalnum(*++ptr));

	if (*ptr)
		flags |= INVALID_LABEL_SYNTAX;

	if (ptr-expr>=MAX_LABEL_LEN)
		flags |= INVALID_LABEL_LEN;

	if (searchOperation(expr)!=FAILURE || isRegister(expr))
		flags |= INVALID_LABEL_NAME;

	return flags;
}
/* ----------------------------------------------------------------	*/

void printLabel(FILE *stream, Label *label)
{
	if (!stream || !label)
		return;

	fprintf(stream, "Type: %-6s\tAddress: %-4hu\tBase address: %-4hu\tOffset:%-4hu\n", 
			LabelTypeStr[getLabelType(label)], getAddress(label),
			getBaseAddress(label), getOffset(label));
}
