#include <addressingModes.h>

/* ----------------------------------------------------------------	*
 *			Constants, Defines, and Structure declarations			*
 * ----------------------------------------------------------------	*/
const char INDEX_DELIMITERS[] = "[]";
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *							Public Functions						*
 * ----------------------------------------------------------------	*/
/* getAddressingMode: Analyzes the expression expr and returns it's
 * corrosponding addressing mode. If expr is a NULL pointer or an empty
 * string then the function will return FAILURE (-1). */
int getAddressingMode(const char *expr)
{
	char *temp;

	if (!expr || !(*expr))
		return FAILURE;

	if (*expr == IMMEDIATE_EXPRESSION_PREFIX)
		return IMMEDIATE;

	if (isRegister(expr))
		return REGISTER_DIRECT;

	if (!(temp=strrchr(expr, INDEX_DELIMITERS[0])) || 
		!(temp=strrchr(temp, INDEX_DELIMITERS[1])))
		return DIRECT;

	return (temp[1]=='\0' ? INDEX: DIRECT);
}

/* getAdditionalMemoryWords: Returns the number of additional memory
 * words required for representing an operand of the given addressingMode */
int getAdditionalMemoryWords(AddressingMode addressingMode)
{
	switch (addressingMode) {
	case IMMEDIATE:
		return 1;

	case DIRECT:
	case INDEX:
		return 2;

	default:
		return 0;
	}
}

/* isRegister: Scans the expression expr and returns 1 if the expression 
 * represents a valid cpu register name, otherwise, returns 0. */
int isRegister(const char *expr)
{
	int16_t tmp = 0;

	return scanRegister(expr, &tmp);
}

/* scanRegister: Scans the expression expr and returns 1 if the expression 
 * represents a valid cpu register name, otherwise, returns 0. 
 * if expr represents a valid cpu register name, then the pointer reg is set 
 * to point to the register number expr refers to. */
int scanRegister(const char *expr, int16_t *reg)
{
	const char testFormat[] = REGISTER_DIRECT_FORMAT" %c";

	char tempC;
	int16_t tempN;
	int scanRes;

	if (!expr || !reg)
		return 0;

	tempN = tempC = 0;
	scanRes	= sscanf(expr, testFormat, &tempN, &tempC);

	if (scanRes!=1 || tempN>MAX_REGISTER_NUMBER || tempN<MIN_REGISTER_NUMBER)
		return 0;

	*reg = tempN;
	return 1;
}


/* scanImmediateExpression: scans an immediate expression and stores 
 * its value in the short integer pointer num.
 * Returns 1 upon success, otherwise, returns 0. */
int scanImmediateExpression(const char *expr, int16_t *num)
{
	const char testFormat[] = IMMEDIATE_FORMAT" %c";

	char tempC;
	int16_t	tempN;
	int scanRes;

	if (!expr || !num)
		return 0;

	tempC = tempN = 0;
	scanRes = sscanf(expr, testFormat, &tempN, &tempC);

	if (scanRes==1) {
		*num = tempN;
		return 1;
	}

	return 0;
}

/* scanIndexExpression: Scans an index addressing mode expression.
 * Returns 1 if expr is indeed a valid index addressing mode expression,
 * otherwise, returns 0.
 * If expr is a valid index expression, then its index's register value 
 * is stored within the short integer pointer reg, and the character pointer
 * expr is modified such that the index delimiter is replaced by a null 
 * terminator so that expr now points to a null terminated label name. */
int scanIndexExpression(char *expr, int16_t *reg)
{
	int labelLen = 0;
	char *regString, *temp;

	if (!expr || !reg)
		return 0;

	if (!(temp=strrchr(expr, INDEX_DELIMITERS[0])))
		return 0;

	regString = temp+1;
	labelLen = temp-expr;

	if (!(temp=strrchr(temp, INDEX_DELIMITERS[1])) || temp[1])
		return 0;

	*temp = '\0';

	if (!scanRegister(regString, reg) || *reg<MIN_INDEX_REGISTER_NUMBER) {
		*temp = INDEX_DELIMITERS[1];
		return 0;
	}

	expr[labelLen] = '\0';
	return 1;
}
/* ----------------------------------------------------------------	*/
