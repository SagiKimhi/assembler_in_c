#include <tempName.h>


enum modeValues {
	LOAD_ADRESSING_MODES(GENERATE_ENUM)
};

/* String formats of each addressing mode */
const char modeFormats[][MAX_ADDRESSING_FORMAT_LEN+1] = {
	/* Immediate */
	"#%hd",			/* For example: #-27 */

	/* Direct */
	"%s",			/* For example: foo */

	/* Index */
	"%s[r%2hd]",	/* For example: foo[r13] */

	/* Register Direct */
	"r%2hd"			/* For example: r5 */

}


const AddressingMode addressingModes[] = {
	/* Immediate Mode */
	{
		IMMEDIATE,						/* Mode Value */
		modeFormats[IMMEDIATE],			/* Mode Format */
		1								/* Additional words required */
	},
	{
		DIRECT,							/* Mode Value */
		modeFormats[Direct],			/* Mode Format */
		2								/* Additional words required */
	},
	{
		INDEX,							/* Mode Value */
		modeFormats[INDEX],				/* Mode Format */
		2								/* Additional words required */
	},
	{
		REGISTER_DIRECT,				/* Mode Value */
		modeFormats[REGISTER_DIRECT],	/* Mode Format */
		0								/* Additional words required */
	},

};

static AddressingMode *searchMode(char *format)
{
	int32_t temp = 0;
	char label[MAX_LABEL_LEN+1] = {0};

	if (!format || strlen(format)>MAX_ADDRESSING_FORMAT_LEN)
		return NULL;

	/* The Direct addressing format is the most generic format,
	 * therefore, it will be the last one we test */
	if (sscanf(format, AddressingModes[IMMEDIATE]->modeFormat, &temp)==1) {
		if (temp>SHRT_MAX || tmp<SHRT_MIN)
	}
}
