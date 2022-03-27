#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H
#include <libraries.h>
#include <labels.h>
#include <addressingModes.h>
#include <operations.h>
#include <binaryTree.h>
#include <libIO.h>
#include <sentences.h>
#include <errors.h>

/* startAssembler: The main assembler function which initiates the first
 * pass of the assembler as well as the second pass if no errors pop up
 * during the first pass. 
 * The function returns a value of EXIT_SUCCESS if both passes completed successfully
 * without any errors, otherwise, EXIT_FAILURE will be returned. 
 * Upon success, a new object file will be created with the OBJECT_FILE_EXTENSION extension.
 * if any entries were defined then an entry file will be created with the ENTRY_FILE_EXTENSION.
 * And if any extern labels were declared and used in an instruction, then an extern file
 * will be created with the EXTERN_FILE_EXTENSION.
 * @param fileName The name of the file to be assembled, without the extension.
 * a post processed '.am' file should already exist before calling this function.
 * @returns EXIT_SUCCESS if completed successfully and EXIT_FALIURE if not. */
int startAssembler(const char *fileName);

#endif
