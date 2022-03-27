#include <assemblerSyntax.h>

extern int isRegister(const char *expr);
extern int searchOperation(const char *expr);

/* isSavedKeyword: Checks if expression is a saved assembler 
 * syntax keyword. Returns 1 if it is or 0 if it isn't. */
int isSavedKeyword(const char *expr)
{
	if (!expr || !*expr)
		return 0;

	return	(	searchOperation(expr)!=FAILURE			||
				isRegister(expr)						||
				!strcmp(expr, START_DATA_DEFINITION)	||
				!strcmp(expr, START_STRING_DEFINITION)	||
				!strcmp(expr, START_ENTRY_DEFINITION)	||
				!strcmp(expr, START_EXTERN_DEFINITION)
			);

}
