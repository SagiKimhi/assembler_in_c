#ifndef _ERRORS_H
#define _ERRORS_H

#define __EXIT_ERROR__MISSING_ARGS(EXECUTABLE_NAME) {\
	fprintf(stderr, "Usage: %s <FILE> <FILE> <...>\n"\
			"Aborting...\n", EXECUTABLE_NAME);\
	exit(EXIT_FAILURE);\
}

#define __ERROR__INVALID_FILE_NAME(FILE_NAME) {\
	fprintf(stderr, "Error: file \"%s\" was not found.\n"\
	"Please make sure the correct file path was provided.\n", FILE_NAME);\
	continue;\
}

#define __ERROR__UNABLE_TO_CREATE_FILE {\
	fprintf(stderr, "Error: unable to open/create a file.\n"\
	"This is an internal program error and is unrelated to the user.\n"\
	"We are truly sorry for the inconvenience.\n");\
	continue;\
}

#endif
