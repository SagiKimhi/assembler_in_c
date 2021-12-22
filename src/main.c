#include <libraries.h>
#include <types.h>
#include <macro.h>
#include <hashTable.h>

int main(int argc, char **argv)
{
	if (argc == 1) {
		fprintf(stderr, "Error!\nPlease provide the names of the files when starting the program");
		return EXIT_FAILURE;
	}
	while (argc>1) {
		FILE *read, *write;
		read = write = NULL;
		read = fopen(argv[argc-1], "r");
		write = fopen("res.as", "w");
		if (!read || !write) {
			fprintf(stderr, "Error!\nInvalid file name: %s", argv[argc-1]);
			return EXIT_FAILURE;
		}
		MacroPreproccessor(read, write);
		fclose(read);
		fclose(write);
		argc--;
	}
	return EXIT_SUCCESS;
}