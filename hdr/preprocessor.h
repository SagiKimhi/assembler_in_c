#ifndef _PREPROCESSOR_H
#define _PREPROCESSOR_H
#include <libIO.h>
#include <macro.h>
#include <libraries.h>
#include <binaryTree.h>

/* macroPreprocessor: opens the provided filename with a '.as' extension for reading,
 * and creates a new file with an identical filename and a '.am' extension for writing.
 * The newly created file will contain the processed contents of the original file, 
 * leaving macro definitions out of the new file and replacing macro calls with 
 * the actual contents of the defined macros.
 * Returns EXIT_SUCCESS upon success or EXIT_FAILURE if an error occured.
 * 
 * @param fileName The name of an already existing '.as' file without its .as
 * extension to be preprocessed by the macro preprocessor. 
 * @return Returns EXIT_SUCCESS upon success or EXIT_FAILURE if an error occured. 
 * A newly processed file with an identical file name and a '.am' extension will
 * be present in the file system if the function executed successfuly. */
int macroPreprocessor(const char *fileName);

#endif
