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
 * The function returns a value of 1 if both passes completed successfully
 * without any errors, otherwise, 0 will be returned.
 * It is worth to mention that warnings do not count as errors and therefore
 * even if a warning was issued, the code will still be assembled and the
 * returned value would still be 1. 
 * @param fileName a name of an already existing '.am' file without the extension 
 * to be assembled 
 * @returns EXIT_SUCCESS if completed successfully and EXIT_FALIURE if not. */
int startAssembler(const char *fileName);

#endif
