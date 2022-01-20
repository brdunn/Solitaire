#include "readInputFile.h"

int main(int argc, char *arg[]) {

    if (argc > 2) {
        fprintf(stderr, "Usage: %s inputfile.\n", arg[0]);
        return -1;
    }

    //opens a FILE* to either a user defined file or stdin
    FILE *inputFile;
    if (argc == 1) {
        inputFile = stdin;
    } else {
        inputFile = fopen(arg[1], "r");

        if (!inputFile) {
            fprintf(stderr, "Could not open file.  File does not exist.\n");
            return -1;
        }
    }

    readInputFile(inputFile);
    printCheckResults();
    fclose(inputFile);
    return 0;
}
