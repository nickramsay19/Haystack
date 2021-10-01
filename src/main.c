#include <stdio.h>
#include <stdlib.h>

#include "include/Stack.h"
#include "include/Parser.h"
#include "include/Executor.h"

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
    //size_t *buf_len;
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

    debug(0.1);    

    while (fgets(buf, MAX_STRING, fp) != NULL) {

        debug(1.0);
        ParseStatement(buf, res);
        debug(2.0);

        Command c = ParseStatement(buf, res);

        debug(723412);

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
            scanf("%d", read);
            stack = StackPush(stack, *read);
            break;
        case PRINT:
            stack = StackPop(stack, pop1);
            printf("%d\n", *pop1); // TODO: replace print with int->char print
            break;
        case PUSH:
            // number to push should be in res
            stack = StackPush(stack, *res);
            break;
        case COPY:
            stack = StackPop(stack, pop1);
            stack = StackPush(stack, *pop1); // undo
            stack = StackPush(stack, *pop1); // copy
            break;
        case POP:
            stack = StackPop(stack, pop1);
            break;
        case ADD:
            stack = StackPop(stack, pop1);
            stack = StackPop(stack, pop2);
            stack = StackPush(stack, *pop1 + *pop2);
            break;
        case SUB: break;
        case MULT:break;
        case DIV:break;
        case MOD:break;
        case BREAK:break;
        // cond start here
        case COND_READ:
            ExecuteConditionally(READ, &stack, &condflag);
            break;
        case COND_PRINT:
            stack = StackPop(stack, pop1);
            stack = StackPop(stack, pop2);
            if (*pop1 && condflag == 0) {
                printf("%d\n", *pop1); // TODO: replace print with int->char print
                condflag = 0;
            } else {
                condflag = 1;
            }
            break;
        case COND_PUSH:
            stack = StackPop(stack, pop1);
            if (pop1) {
                stack = StackPush(stack, *res);
            }
            break;
        case COND_COPY:
            stack = StackPop(stack, pop1);
            if (pop1) {
                stack = StackPop(stack, pop1);
                stack = StackPush(stack, *pop1); // undo
                stack = StackPush(stack, *pop1); // copy
            }
            break;
        case COND_POP:
            stack = StackPop(stack, pop1);
            if (pop1) {
                stack = StackPop(stack, NULL); // todo: check if null ok
            }
            break;
        case COND_ADD:
            stack = StackPop(stack, pop1);
            if (pop1) {
                stack = StackPop(stack, pop1);
                stack = StackPop(stack, pop2);
                stack = StackPush(stack, *pop1 + *pop2);
            }
            break;
        case COND_SUB: break;
        case COND_MULT:break;
        case COND_DIV:break;
        case COND_MOD:break;
        case COND_BREAK:break;
        
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