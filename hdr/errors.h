#ifndef _ERRORS_H
#define _ERRORS_H

#include <libraries.h>

#define __ERROR__MISSING_ARGS(EXECUTABLE_NAME) {\
	fprintf(stderr, "Usage: %s <FILE> <FILE> <...>\n"\
			"Aborting...\n", EXECUTABLE_NAME);\
}

#define __ERROR__INVALID_FILE_NAME(FILE_NAME) {\
	fprintf(stderr, "Error: file \"%s\" was not found.\n"\
	"Please make sure the correct file path was provided.\n", FILE_NAME);\
}

#define __ERROR__UNABLE_TO_CREATE_FILE {\
	fprintf(stderr, "Error: unable to open/create a file.\n"\
	"This is an internal program error and is unrelated to the user.\n"\
	"We are truly sorry for the inconvenience.\n");\
}

#define __ERROR__FILE_NAME_TOO_LONG(FILE_NAME) {\
	fprintf(stderr, "Error: File \"%s\" exceeded the max length allowed for a file name \n"\
			"(%d)", FILE_NAME, FILENAME_MAX-MAX_FILE_EXTENSION_LEN);\
}

#endif
