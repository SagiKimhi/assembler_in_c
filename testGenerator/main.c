#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    unsigned int count;
    const unsigned int macros = atoi(argv[argc-1]);
    
    if (macros<=0) {
        fprintf(stderr, "Argument has to be a positive integer. Aborting...");
        return EXIT_FAILURE;
    }

    for (count=0; count<macros; count++)
        printf("macro m%d\n inc r6\n mov r3, W\nendm\n", count);

    for (count=0; count<macros; count++)
        printf("m%d\n", count);

    return 0;
}
