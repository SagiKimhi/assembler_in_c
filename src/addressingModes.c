#include <addressingModes.h>

const char immediateFormat[] = "#%d";		/* for example: #-536 */
const char directFormat[] = GENERATE_SCANF_FORMAT(MAX_LABEL_LEN,"[^[]");
const char registerDirectFormat[] = "[r%d]";
const char indexFormat[] = directFormat""registerDirectFormat;

AddressingMode_t getAddressingMode(const char *expr)
{
	return	(isImmediateMode(expr)) ? IMMEDIATE:
			(isRegisterDirectMode(expr)) ? REGISTER_DIRECT:
			(isIndexMode(expr)) ? INDEX :
			(isDirectMode(expr)) ? DIRECT:
			-1;
}

/* isImmediateMode: checks if expr is a valid immediate addressing mode expression.
 * Returns 1 if it is, 0 if it isn't. */
int isImmediateMode(const char *expr)
{
	char tempC;
	int tempN, scanRes;
	char testFormat[] = immediateFormat" %c"	/* %c is added to ensure no additional characters
												exist after the scanned number */

	if (!expr)
		return 0;

	tempC = tempN = 0;
	scanRes = sscanf(expr, testFormat, &tempN, &tempC);

	if (scanRes==1)
		return 1;

	return 0;
}

int isDirectMode(const char *expr)
{
	int scanRes;
	char tempStr[MAX_LABEL_LEN+1] = {0}, tempC = 0;
	char testFormat[] = directFormat" %c";

	if (!expr)
		return 0;

	scanRes = sscanf(expr, testFormat, tempStr, &tempC);

	if (scanRes==1)
		return 1;

	return 0;
}

int isIndexMode(const char *expr)
{
	const int INDEX_REG_MAX 15;
	const int INDEX_REG_MIN 12;

	int tempN, scanRes;
	char testFormat[] = indexFormat" %c";
	char tempStr[MAX_ADDRESSING_FORMAT_LEN+1] = {0}, tempC;

	if (!expr)
		return 0;

	tempN = tempC = 0;
	scanRes = sscanf(expr, testFormat, tempStr, tempN, &tempC);

	if (scanRes == 2) {
		if (tempN>=INDEX_REG_MIN && tempN<=INDEX_REG_MAX)
	}

	return 0;
}

int isRegisterDirectMode(const char *expr)
{
	int tempN, scanRes;
	char tempC;
	char testFormat[] = registerDirectFormat" %c";

	if (!expr || isdigit(expr[0]))
		return 0;

	tempN = tempC = 0;
	scanRes = sscanf(expr, testFormat, &tempN, &tempC);

	if (scanRes == 1)
		return 1;

	return 0;
}
