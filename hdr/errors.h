#ifndef _ERRORS_H
#define _ERRORS_H

enum Errors{
	MISSING_ARGUMENTS,
	INVALID_FILE_NAME,
	UNABLE_TO_OPEN_FILE
};

#define EXIT_ERROR(ERROR, OPTIONAL_FILE_NAME) {\
	switch (ERROR) {\
	case MISSING_ARGUMENTS:\
		fprintf(stderr, "Error: No command line arguments were found. "\
		"Please provide the paths to the files which you wish to assemble"\
		"as command line arguments...\nAborting operation...\n");\
		exit(EXIT_FAILURE);\
		\
	case INVALID_FILE_NAME:\
		fprintf(stderr, "Error: file \"%s\" was not found. "\
		"\nPlease ensure you are providing the correct file path and try again.\n"\
		"Aborting...\n", OPTIONAL_FILE_NAME);\
		exit(EXIT_FAILURE);\
		\
	case UNABLE_TO_OPEN_FILE:\
		fprintf(stderr, "Error: unable to open/create a file. "\
		"This is an internal program error and is unrelated to the user.\n"\
		"We are truly sorry for the inconvenience. Aborting...\n");\
		exit(EXIT_FAILURE);\
	}\
}
#endif
