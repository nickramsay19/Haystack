#include <stdio.h>
#include <stdlib.h>

#include "include/Parser.h"
#include "include/Stack.h"
#include "include/Runtime.h"
#include "include/Executor.h"

// method headers
int ExecuteConditionally(Command c, Runtime runtime);
int ExecuteLoop(Command c, Runtime runtime);
int ExecuteJump(Command c, Runtime runtime);
int Execute(Command c, Runtime runtime);

int DelegateExecution(Command c, Runtime runtime) {
    switch (c)
    {
    case TYPE_ERROR:
        runtime->error = true;
        printf("Error: Type error (line: %d) in execution.\n", runtime->line_num);
        return 0;
    case SYNTAX_ERROR:
        runtime->error = true;
        printf("Error: Syntax error (line: %d) in execution.\n", runtime->line_num);
        return 0;
    case COND_ERROR:
        runtime->error = true;
        printf("Error: Conditional error (line: %d) in execution.\nDid you put a conditional statement within a conditional statement?\n", runtime->line_num);
    case READ:
    case PRINT:
    case PUSH:
    case COPY:
    case POP:
    case ADD:
    case SUB: 
    case MULT:
    case DIV:
    case MOD:
        Execute(c, runtime);
        break;
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
    case COND_SUB:
        ExecuteConditionally(SUB, runtime);
        break;
    case COND_MULT:
        ExecuteConditionally(MULT, runtime);
        break;
    case COND_DIV:
        ExecuteConditionally(DIV, runtime);
        break;
    case COND_MOD:
        ExecuteConditionally(MOD, runtime);
        break;
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
    case LOOP:
        ExecuteLoop(c, runtime);
    case THEN_BREAK:
        Execute(c, runtime);
        break;
    case JUMP:
        if (runtime->then || runtime->cond) {
            ExecuteConditionally(c, runtime);
        } else {
            ExecuteJump(c, runtime);
        }
        
        break;
    case NONE:
        break;
    default:
        runtime->error = true;
        printf("Error: Unknown error (line: %d) in execution.\nHave you entered a nonexistent command?\n", runtime->line_num);
        return 0;
    }

    // increment line number
    runtime->line_num++;

    return 1;
}

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

    } else if (c == JUMP) {
        runtime->loop_depth--;
    }

    free(pop);

    return 1;
}

int ExecuteLoop(Command c, Runtime runtime) {
    // edit runtime
    runtime->loop_depth++;
    runtime->loop_reference[runtime->loop_depth] = runtime->line_num;

    return 1;
}

int ExecuteJump(Command c, Runtime runtime) {

    // set runtime line number
    runtime->line_num = runtime->loop_reference[runtime->loop_depth];// + 1;

    return 1;
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
        runtime->stack = StackPush(runtime->stack, *pop2 - *pop1);
        break;
    case MULT:
        runtime->stack = StackPop(runtime->stack, pop1);
        runtime->stack = StackPop(runtime->stack, pop2);
        runtime->stack = StackPush(runtime->stack, *pop1 * *pop2);
        break;
    case DIV:
        runtime->stack = StackPop(runtime->stack, pop1);
        runtime->stack = StackPop(runtime->stack, pop2);
        if (*pop1 != 0) {
            runtime->stack = StackPush(runtime->stack, *pop2 / *pop1);
        } else {
            return 0;
        }
        break;
    case MOD:
        runtime->stack = StackPop(runtime->stack, pop1);
        runtime->stack = StackPop(runtime->stack, pop2);
        if (*pop1 != 0) {
            runtime->stack = StackPush(runtime->stack, *pop2 % *pop1);
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
    case JUMP:
        ExecuteJump(c, runtime);
        break;
    default:
        break;
    }

    free(pop1);
    free(pop2);
    free(read);
    free(res);

    return 1;
}