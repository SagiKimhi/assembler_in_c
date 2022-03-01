#include <addressingModes.h>


/* isImmediateMode: checks if expr is a valid immediate addressing mode expression.
 * Returns 1 if it is, 0 if it isn't. */
int isImmediateMode(const char *expr, int *number)
{
	const char testFormat[] = IMMEDIATE_FORMAT" %c";

	char tempC;
	int16_t	tempN;
	int scanRes;

	if (!expr || !number)
		return 0;

	tempC = tempN = 0;
	scanRes = sscanf(expr, testFormat, &tempN, &tempC);

	if (scanRes==1) {
		*number = tempN;
		return 1;
	}

	return 0;
}

int isDirectMode(const char *expr)
{
	if (!expr)
		return 0;

	return isValidLabelName(expr);
}

int isIndexMode(const char *expr, char destLabel[MAX_LABEL_LEN+1], int *reg)
{
	const char delims[] = "[]";

	int labelLen, regLen;
	char *regStart, *regEnd;
	char tempReg[MAX_REGISTER_LEN+1];

	if (!expr || !reg)
		return 0;

	labelLen = strcspn(expr, delims);
	regStart = &expr[labelLen];

	if (labelLen>MAX_LABEL_LEN || regStart[0]!='[')
		return 0;

	regStart++;
	regLen = strcspn(regStart, delims);
	regEnd = &regStart[regLen];

	if (regEnd[0]!=']' || regEnd[1]!='\0')
		return 0;

	if (!strncpy(destLabel, expr, labelLen) || !isDirectMode(destLabel))
		return 0;

	if (!strncpy(tempReg, regStart, regLen) || !isRegisterDirectMode(expr, reg))
		return 0;
	
	if ((*reg)<MIN_INDEX_REGISTER_NUMBER || (*reg)>MAX_REGISTER_NUMBER)
		return 0;
		
	return 1;
}

int isRegisterDirectMode(const char *expr, int *reg)
{
	const char testFormat[] = REGISTER_DIRECT_FORMAT" %c";

	char tempC;
	int16_t tempN;
	int scanRes;

	if (!expr || !reg)
		return 0;

	tempN = tempC = 0;
	scanRes	= sscanf(expr, testFormat, &tempN, &tempC);

	if (scanRes == 1) {
		if (tempN>MAX_REGISTER_NUMBER || tempN<MIN_REGISTER_NUMBER)
				return 0;

		*reg = tempN;
		return 1;
	}

	return 0;
}

int getAdditionalMemoryWords(AddressingMode_t mode)
{
	switch (mode) {
	case IMMEDIATE:
		return 1;

	case DIRECT:
	case INDEX:
		return 2;

	case REGISTER_DIRECT:
		return 0;
	}
}
