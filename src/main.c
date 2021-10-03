#include <stdio.h>
#include <stdlib.h>

#include "include/Stack.h"
#include "include/Parser.h"
#include "include/Executor.h"
#include "include/Runtime.h"

#define MAX_STRING 1000

void printStack(Stack s); // delete me

void debug(float x) {
    //printf("%f\n", x);
}

int main(int argc, char **argv) {
    
    if (argc < 2) {
        printf("Error: No input file specified.\n");
        return 1;
    }

    // open the file
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: File \"%s\" could not be found.\n", filename);
        return 1;
    }

    // loop through the file
    char buf[MAX_STRING];

    // initalise runtime state
    Runtime runtime = RuntimeNew();

    while (fgets(buf, MAX_STRING, fp) != NULL) {

        // parse statment to a command, collect runtime flag changes for pre-execution
        Command c = ParseStatement(buf, runtime);

        // delegate execution amongst executor
        DelegateExecution(c, runtime);
    }
    
    // free memory, close files
    fclose(fp);
    RuntimeFree(runtime);

    return 0;
}