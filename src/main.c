#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/Stack.h"
#include "include/Errors.h"
#include "include/Parser.h"
#include "include/Executor.h"
#include "include/Runtime.h"

#define MAX_STRING 1000

void printStack(Stack s); // delete me

void debug(float x) {
    //printf("%f\n", x);
}

void setLine(FILE *fp, int line) {
    fseek(fp, SEEK_SET, 0);

    int c;

    int i = 1;
    while (i < line) {
        c = fgetc(fp);
        if (c == '\n') {
            i++;
        } else if (c == EOF) {
            break;
        }
    }
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

    char *fg;// = fgets(buf, MAX_STRING, fp);

    while (fgets(buf, MAX_STRING, fp) != NULL) {

        // parse statment to a command, collect runtime flag changes for pre-execution
        Command c = ParseStatement(buf, runtime);

        // execute if no parsing errors detected
        if (runtime->error_type == ERROR_NONE) {
            DelegateExecution(c, runtime);
        }

        // handle errors caught in parsing and execution
        if (runtime->error_type != ERROR_NONE) {
            HandleError(runtime);
            break;
        }

        // move to the correct line
        setLine(fp, runtime->line_num);
    }
    
    // free memory, close files
    fclose(fp);
    RuntimeFree(runtime);

    return 0;
}