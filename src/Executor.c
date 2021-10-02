#include <stdio.h>
#include <stdlib.h>

#include "include/Parser.h"
#include "include/Stack.h"
#include "include/Runtime.h"
#include "include/Executor.h"


int ExecuteConditionally(Command c, Runtime runtime) {
    int *pop = malloc(sizeof(int));

    // set the carry flag, we assume we wont carry
    runtime->cond_carry = 0;

    runtime->stack = StackPop(runtime->stack, pop);

    if (*pop) {

        Execute(c, runtime);

        // set the contional carry flag
        runtime->cond_carry = 1;

        // end the conditional sequence
        runtime->cond = 0;
    }

    free(pop);
}

int Execute(Command c, Runtime runtime) {

    int *pop1 = malloc(sizeof(int));
    int *pop2 = malloc(sizeof(int));
    int *read = malloc(sizeof(int));
    int *res = malloc(sizeof(int));

    switch (c) {
    case READ:
        scanf("%d", read);
        runtime->stack = StackPush(runtime->stack, *read);
        break;
    case PRINT:
        runtime->stack = StackPop(runtime->stack, pop1);
        printf("%d\n", *pop1); // TODO: replace print with int->char print
        break;
    case PUSH:
        // number to push should be in res
        runtime->stack = StackPush(runtime->stack, runtime->payload[0]);
        break;
    case COPY:
        runtime->stack = StackPop(runtime->stack, pop1);
        runtime->stack = StackPush(runtime->stack, *pop1); // undo
        runtime->stack = StackPush(runtime->stack, *pop1); // copy
        break;
    case POP:
        runtime->stack = StackPop(runtime->stack, pop1);
        break;
    case ADD:
        runtime->stack = StackPop(runtime->stack, pop1);
        runtime->stack = StackPop(runtime->stack, pop2);
        runtime->stack = StackPush(runtime->stack, *pop1 + *pop2);
        break;
    case SUB: 
        runtime->stack = StackPop(runtime->stack, pop1);
        runtime->stack = StackPop(runtime->stack, pop2);
        runtime->stack = StackPush(runtime->stack, *pop1 - *pop2);
        break;
    case MULT:
        runtime->stack = StackPop(runtime->stack, pop1);
        runtime->stack = StackPop(runtime->stack, pop2);
        runtime->stack = StackPush(runtime->stack, *pop1 * *pop2);
        break;
    case DIV:
        runtime->stack = StackPop(runtime->stack, pop1);
        runtime->stack = StackPop(runtime->stack, pop2);
        if (*pop2 != 0) {
            runtime->stack = StackPush(runtime->stack, *pop1 / *pop2);
        } else {
            return 0;
        }
        break;
    case MOD:
        runtime->stack = StackPop(runtime->stack, pop1);
        runtime->stack = StackPop(runtime->stack, pop2);
        if (*pop2 != 0) {
            runtime->stack = StackPush(runtime->stack, *pop1 % *pop2);
        } else {
            return 0;
        }
        break;
    case THEN_READ:
        if (runtime->cond_carry) {
            Execute(READ, runtime);
        }
        break;
    case THEN_PRINT:
        if (runtime->cond_carry) {
            Execute(PRINT, runtime);
        }
        break;
    case THEN_PUSH:
        if (runtime->cond_carry) {
            Execute(PUSH, runtime);
        }
        break;
    case THEN_COPY:
        if (runtime->cond_carry) {
            Execute(COPY, runtime);
        }
        break;
    case THEN_POP:
        if (runtime->cond_carry) {
            Execute(POP, runtime);
        }
        break;
    case THEN_ADD:
        if (runtime->cond_carry) {
            Execute(ADD, runtime);
        }
        break;
    case THEN_SUB: 
        if (runtime->cond_carry) {
            Execute(SUB, runtime);
        }
        break;
    case THEN_MULT:
        if (runtime->cond_carry) {
            Execute(MULT, runtime);
        }
        break;
    case THEN_DIV:
        if (runtime->cond_carry) {
            Execute(DIV, runtime);
        }
        break;
    case THEN_MOD:
        if (runtime->cond_carry) {
            Execute(MOD, runtime);
        }
        break;
    case BREAK:break;
    }

    free(pop1);
    free(pop2);
    free(read);
    free(res);

    return 1;
}