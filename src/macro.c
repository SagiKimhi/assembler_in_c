/*  
*	if (feof(file))
*		return 1;
*	scan the line and look for the word syntax macro
*	if (the macro word was found) {
*		if (the following word already exists in our table)
*			ignore everything until "endm" is reached;
*		else {
*			save the following word into a table (see table description later on...);
*			the name of the macro will be the head of a list;
*			while (the next word is not equal to "endm")
*				set the next character combination as the next element in the list;
*		}
*	}
*	else if (the current word exists in our macro table/array) {
*		go to the next element it points to within the table;
*		while (nextElement != NULL)
*			write it into the text file;
*	}
*	write the line into the output file and return to the begining of the funct;
*	
*	For the purpose of the functionality of this source code we will need:
*	1. A dynamic array of (dynamic arrays/pointers to linked lists)
*	2. A string hashtable implementation (maybe stringTable.c/hashTable.c?)
*	3. A counter to keep track of memory usage.
*
*/
#include <macro.h>
#include <linkedList.h>
#include <hashTable.h>

Macro *newMacro(char *text, fpos_t position)
{
	Macro *ptr = NULL;
	if (!text)
		return NULL;
	if(!(ptr = (Macro *)malloc(sizeof(Macro))))
		return NULL;
	ptr->key = (char *)malloc(sizeof(char)*(strlen(text)+1));
	if (!ptr->key) {
		free(ptr);
		return NULL;
	}
	strcpy(ptr->key, text);
	ptr->fStartPos = position;
	return ptr;
}

void deleteMacro(Macro *macro)
{
	if (!macro)
		return;
	free(macro->key);
	free(macro);
	macro = NULL;
}