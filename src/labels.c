#include <labels.h>

static void setBaseAddress(Label *label, uint16_t address);
static void setOffset(Label *label, uint16_t address);

struct label {
	uint16_t baseAddress;
	uint16_t offset;
	LabelType type;
};

/* ----------------------------------------------------------------	*
 *							De/Constructor							*
 * ----------------------------------------------------------------	*/
/* newLabel: Allocates a new label object in memory and sets its base address
 * and offset according to address argument.
 * Returns a pointer to the new object upon success, or NULL upon failure. */
Label *newLabel(uint16_t address, LabelType type)
{
	Label *newp = (Label *) malloc(sizeof(Label));

	if (!newp)
		return NULL;

	setAddress(newp, address);
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
/* setAddress: update label's base address and offset according to address */
void setAddress(Label *label, uint16_t address)
{
	if (!label)
		return;

	setBaseAddress(label, address);
	setOffset(label, address);
}

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
/* getAddress: Returns label's address */
uint16_t getAddress(Label *label)
{
	return (getBaseAddress(label) + getOffset(label));
}

/* getBaseAddress: Returns the base address of the label.
 * Returns 0 if the argument is a NULL pointer. */
uint16_t getBaseAddress(Label *label)
{
	if (!label)
		return 0;

	return label->baseAddress;
}

/* getOffset: Returns the offset of the label from the base address to the label's
 * actual address. Returns 0 if the argument is a NULL pointer. */
uint16_t getOffset(Label *label)
{
	if (!label)
		return 0;

	return label->offset;
}

LabelType getType(Label *label)
{
	return label->type;
}
/* ----------------------------------------------------------------	*/


/* ----------------------------------------------------------------	*
 *						Additional Functions						*
 * ----------------------------------------------------------------	*/
/* isValidLabelName: Checks if the expression is a valid label name.
 * Returns 0 if it is, 
 * otherwise returns one of the following nonzero values:
 *	   -1: A null pointer argument
 *		1: Label length is longer than the constant MAX_LABEL_LEN 
 *		2: Invalid label character 
 *		3: Label name is a saved keyword */
int isValidLabelDefinition(const char *expr)
{
	int i=0;
	char labelName[MAX_LABEL_LEN+1] = {0};

	if (!expr || !(*expr))
		return FAILURE;

	if (strlen(expr)>MAX_LABEL_LEN)
		return 1;

	/* The following conditions ensure that the label's name is completely 
	 * alphanumeric and that the first character is alphabetic: */
	if (!isalpha(*expr))
		return 2;

	while (*expr && isalnum(*expr))
		labelName[i++] = *expr++;
	
	labelName[i] = '\0';

	if (*expr!=LABEL_DEFINITION_SUFFIX || *(expr+1)!='\0')
		return 2;

	/* Make sure the scanned label name is not a saved keyword */
	if (searchOperation(labelName)!=FAILURE || isRegister(labelName))
		return 3;

	return 0;
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *							Static Functions						*
 * ----------------------------------------------------------------	*/
/* setBaseAddress: A private function to calculate and set the Label's 
 * base address from a given address */
static void setBaseAddress(Label *label, uint16_t address)
{
	if (!address) {
		label->baseAddress = 0;
		return;
	}

	label->baseAddress = (address/ADDRESS_BASE)*ADDRESS_BASE;
	
	if (address==label->baseAddress)
		label->baseAddress -= ADDRESS_BASE;
}

/* setOffset: A private function to calculate and set the label's 
 * offset from the label's base address to the given address */
static void setOffset(Label *label, uint16_t address)
{
	label->offset = address - label->baseAddress;
}
/* ----------------------------------------------------------------	*/

