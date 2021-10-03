#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/Stack.h"
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

    char *fg = fgets(buf, MAX_STRING, fp);

    while (fg != NULL && !runtime->error) {

        // parse statment to a command, collect runtime flag changes for pre-execution
        Command c = ParseStatement(buf, runtime);

        //printf("finished parser\n");

        // delegate execution amongst executor
        DelegateExecution(c, runtime);
        //printf("finished execution\n");
        //printf("error flag = %d, line = %d\n", runtime->error, runtime->line_num);

        // move to the correct line
        //fseek(fp, SEEK_SET, runtime->line_num);
        setLine(fp, runtime->line_num);
        //printf("jumped to line %d\n", runtime->line_num);

        //printf("retriveding next line:\n");
        fg = fgets(buf, MAX_STRING, fp);
        
        //printf("fg null: %s\n", fg == NULL ? "true" : "false");

        //printf("eof: %s\n", feof(fp) ? "true" : "false");
        //printf("ferror: %s\n", ferror(fp) ? "true" : "false");

        //printf("printing fg:\n");
        //printf("\nfinihsed printing fg\n");
    }
    
    // free memory, close files
    fclose(fp);
    RuntimeFree(runtime);

    return 0;
}