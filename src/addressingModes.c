#include <addressingModes.h>

const char INDEX_DELIMITERS[] = "[]";

int getAdditionalMemoryWords(AddressingMode mode)
{
	switch (mode) {
	case IMMEDIATE:
		return 1;

	case DIRECT:
	case INDEX:
		return 2;

	default:
		return 0;
	}
}

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

int isRegister(const char *expr)
{
	int tmp = 0;

	return scanRegister(expr, &tmp);
}

int scanRegister(const char *expr, int *reg)
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


/* scanImmediateExpression: scans an immediate expression's value into num.
 * Returns 1 upon success, otherwise, returns 0. */
int scanImmediateExpression(const char *expr, int *num)
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

int scanIndexExpression(char *expr, int *reg)
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

	if (!scanRegister(regString, reg)) {
		*temp = INDEX_DELIMITERS[1];
		return 0;
	}

	expr[labelLen] = '\0';
	return 1;
}
