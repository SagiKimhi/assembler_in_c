#include <libraries.h>
#include <preprocessor.h>

#define FILE_NAME_MAX 1024
int main(int argc, char **argv)
{
	char readName[FILE_NAME_MAX], writeName[FILE_NAME_MAX];
	int count=1;

	if (argc == 1) {
		fprintf(stderr, "Error!\nPlease provide the names of the files when starting the program");
		return EXIT_FAILURE;
	}
	while (argc>1) {
		FILE *read, *write;
		
		read = write = NULL;
		sscanf(argv[argc-1], " %s", readName);
		sprintf(writeName, "res.as");
		read = fopen(readName, "r");
		write = fopen(writeName, "w");
		
		if (!read || !write) {
			fprintf(stderr, "Error: invalid file name: %s\n", readName);
			return EXIT_FAILURE;
		}
		
		MacroPreproccessor(read, write);
		fclose(read);
		fclose(write);
		argc--;
		count++;
	}

	return EXIT_SUCCESS;
}

