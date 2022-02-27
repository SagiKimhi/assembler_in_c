#include <addressingModes.h>


/* isImmediateMode: checks if expr is a valid immediate addressing mode expression.
 * Returns 1 if it is, 0 if it isn't. */
int isImmediateMode(const char *expr)
{
	char tempC;
	int tempN, scanRes;
	char immediateFormat[] = " #%d ";		/* for example: #-536 */
	char testFormat[] = immediateFormat"%c"	/* %c is added to ensure no additional characters
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
	char directFormat[] = GENERATE_SCANF_FORMAT(MAX_LABEL_LEN,'s');
	char testFormat[] = directFormat"%c";

	if (!expr)
		return 0;

	scanRes = sscanf(expr, testFormat, tempStr, &tempC);

	if (scanRes==1)
		return 1;

	return 0;
}
