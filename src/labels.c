#include <labels.h>

struct label {
	uint16_t address;
	uint16_t baseAddress;
	uint16_t offset;
};

/* -------------------------
 * De/Constructor:
 * ------------------------- */
Label *newLabel(uint16_t address)
{
	Label *newp = (Label *) malloc(sizeof(Label));

	if (!newp)
		return NULL;

	setAddress(newp, address);
	return newp;
}

void deleteLabel(Label *label)
{
	free(label);
}

/* -------------------------
 * Setters:
 * ------------------------- */
static void setBaseAddress(Label *label);
static void setOffset(Label *label);

void setAddress(Label *label, uint16_t address)
{
	if (!label)
		return;

	label->address = address;
	setBaseAddress(label);
	setOffset(label);
}

/* -------------------------
 * Getters:
 * ------------------------- */
uint16_t getAddress(Label *label)
{
	if (!label)
		return 0;

	return label->address;
}

uint16_t getBaseAddress(Label *label)
{
	if (!label)
		return 0;

	return label->baseAddress;
}

uint16_t getOffset(Label *label)
{
	if (!label)
		return 0;

	return label->offset;
}

/* -------------------------
 * Additional Functions:
 * ------------------------- */
int isValidLabelName(const char *expr)
{
	if (!expr || !(*expr))
		return 0;

	if (searchOperation(expr)!=FAILURE || isRegister(expr))
		return 0;

	if (!isalpha(*expr))
		return 0;

	while (*expr && isalnum(*expr))
		expr++;
	
	if (*expr==':' && *++expr=='\0')
		return 1;

	return 0;
}

/* -------------------------
 * Static Functions:
 * ------------------------- */
static void setBaseAddress(Label *label)
{
	if (!label->address) {
		label->baseAddress = 0;
		return;
	}

	label->baseAddress = (label->address>>LOG_2_ADDRESS_BASE)<<LOG_2_ADDRESS_BASE;
	
	if (label->address==label->baseAddress)
		label->baseAddress -= ADDRESS_BASE;
}

static void setOffset(Label *label)
{
	label->offset = label->address - label->baseAddress;
}

