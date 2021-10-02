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

    // initialise the stack
    Stack stack = StackNew(0);

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

    int *res = malloc(sizeof(int));
    int line = 1;
    int *read = malloc(sizeof(int));
    int *pop1 = malloc(sizeof(int));
    int *pop2 = malloc(sizeof(int));
    int *pop3 = malloc(sizeof(int));
    int condflag = 0;

    debug(0.0);

    //fscanf(fp, "%s", buf);
    //fgets(buf, MAX_STRING, fp);
   

    while (fgets(buf, MAX_STRING, fp) != NULL) {

        ParseStatement(buf, runtime);
        debug(2.0);

        Command c = ParseStatement(buf, runtime);

        debug(723412);

        // DelegateExecution(c, runtime)

        switch (c)
        {
        case TYPE_ERROR:
            printf("Error: Type error (line: %d).\n", line);
            return 0;
        case SYNTAX_ERROR:
            printf("Error: Syntax error (line: %d). At \"%s\".\n", line, buf);
            return 0;
        case COND_ERROR:
            printf("Error: Conditional error (line: %d). At \"%s\".\nDid you put a conditional statement within a conditional statement?\n", line, buf);
        case READ:
        case PRINT:
        case PUSH:
        case COPY:
        case POP:
        case ADD:
            Execute(c, runtime);
            break;
        case SUB: break;
        case MULT:break;
        case DIV:break;
        case MOD:break;
        case BREAK:break;
        // cond start here
        case COND_READ:
            ExecuteConditionally(READ, runtime);
            break;
        case COND_PRINT:
            ExecuteConditionally(PRINT, runtime);
            break;
        case COND_PUSH:
            ExecuteConditionally(PUSH, runtime);
            break;
        case COND_COPY:
            ExecuteConditionally(COPY, runtime);
            break;
        case COND_POP:
            ExecuteConditionally(POP, runtime);
            break;
        case COND_ADD:
            ExecuteConditionally(ADD, runtime);
            break;
        case COND_SUB: break;
        case COND_MULT:break;
        case COND_DIV:break;
        case COND_MOD:break;
        case COND_BREAK:break;
        case THEN_READ:
        case THEN_PRINT:
        case THEN_PUSH:
        case THEN_COPY:
        case THEN_POP:
        case THEN_ADD:
        case THEN_SUB:
        case THEN_MULT:
        case THEN_DIV:
        case THEN_MOD:
        case THEN_BREAK:
            Execute(c, runtime);

        case NONE:
            break;
        default:
            printf("Error: Unknown error (line: %d).\nHave you entered a nonexistent command?\n", line);
            return 0;
        }

        // increment line number
        line++;
    }
    
    // free memory
    free(res);
    free(read);
    free(pop1);
    free(pop2);
    free(pop3);
    fclose(fp);

    return 0;
}