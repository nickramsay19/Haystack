#include <stdio.h>
#include <stdlib.h>

#include "include/Parser.h"
#include "include/Stack.h"
#include "include/Executor.h"

int ExecuteConditionally(Command c, Stack *s, int *condflag) {
    int *pop = malloc(sizeof(int));

    *s = StackPop(*s, pop);

    if (*pop && !*condflag) {

        Execute(c, s);

        // end the conditional sequence
        *condflag = 0;
    } else {

        // continue the conditional sequence
        *condflag = 0;
    }

    free(pop);
}

int Execute(Command c, Stack *s) {

    int *pop1 = malloc(sizeof(int));
    int *pop2 = malloc(sizeof(int));
    int *read = malloc(sizeof(int));

    switch (c) {
    case READ:
        scanf("%d", read);
        *s = StackPush(*s, *read);
        break;
    case PRINT:
        *s = StackPop(*s, pop1);
        printf("%d\n", *pop1); // TODO: replace print with int->char print
        break;
    case PUSH:
        // number to push should be in res
        *s = StackPush(*s, *res);
        break;
    case COPY:
        *s = StackPop(*s, pop1);
        *s = StackPush(*s, *pop1); // undo
        *s = StackPush(*s, *pop1); // copy
        break;
    case POP:
        *s = StackPop(*s, pop1);
        break;
    case ADD:
        *s = StackPop(*s, pop1);
        *s = StackPop(*s, pop2);
        *s = StackPush(*s, *pop1 + *pop2);
        break;
    case SUB: 
        *s = StackPop(*s, pop1);
        *s = StackPop(*s, pop2);
        *s = StackPush(*s, *pop1 - *pop2);
        break;
    case MULT:
        *s = StackPop(*s, pop1);
        *s = StackPop(*s, pop2);
        *s = StackPush(*s, *pop1 * *pop2);
        break;
    case DIV:
        *s = StackPop(*s, pop1);
        *s = StackPop(*s, pop2);
        if (*pop2 != 0) {
            *s = StackPush(*s, *pop1 / *pop2);
        } else {
            return 0;
        }
        
        break;
    case MOD:
        *s = StackPop(*s, pop1);
        *s = StackPop(*s, pop2);
        if (*pop2 != 0) {
            *s = StackPush(*s, *pop1 % *pop2);
        } else {
            return 0;
        }
        
        break;
    case BREAK:break;
    }

    free(pop1);
    free(pop2);
    free(read);

    return 1;
}