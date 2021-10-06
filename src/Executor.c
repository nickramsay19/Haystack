#include <stdio.h>
#include <stdlib.h>

#include "include/Parser.h"
#include "include/Stack.h"
#include "include/Runtime.h"
#include "include/Executor.h"

#define BUFFER_SIZE 3

// method headers
int ExecuteConditionally(Command c, Runtime runtime);
int ExecuteLoop(Command c, Runtime runtime);
int ExecuteJump(Command c, Runtime runtime);
int Execute(Command c, Runtime runtime);

int DelegateExecution(Command c, Runtime runtime) {

    // first check the then flag
    if (runtime->then && !runtime->cond_carry) {
        // increment line number
        runtime->line_num++;
        return 0;
    } else if (!runtime->then) {
        // if the statement isn't a then, ensure no conds carry forward
        runtime->cond_carry = false;
    }

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
    case LOOP:
        ExecuteLoop(c, runtime);
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

    int *buffer = calloc(BUFFER_SIZE, sizeof(int));
    char *read = malloc(sizeof(char));

    // set the carry flag, we assume we wont carry
    runtime->cond_carry = 0;

    if (runtime->cond) {
        runtime->stack = StackPop(runtime->stack, &buffer[0]);
        bool cond = buffer[0];

        // check if the condition passes to execute
        if (cond && c == JUMP) {

            // set the contional carry flag
            runtime->cond_carry = 1;

            // end the conditional sequence
            runtime->cond = 0;
        
        // check for failed jump on failed cond
        } else if (c == JUMP) {
            runtime->loop_depth--;
            return 0; // dont execute
            
        } else {
            return 0; // dont execute
        }
    }

    switch (c) {
    case READ:
        scanf("%c", read);
        runtime->stack = StackPush(runtime->stack, (int) *read);
        break;
    case PRINT:
        runtime->stack = StackPop(runtime->stack, &buffer[0]);
        printf("%c", (char) buffer[0]);
        break;
    case PUSH:
        // number to push should be in res
        runtime->stack = StackPush(runtime->stack, runtime->payload[0]);
        break;
    case COPY:
        runtime->stack = StackPop(runtime->stack, &buffer[0]);
        runtime->stack = StackPush(runtime->stack, buffer[0]); // undo
        runtime->stack = StackPush(runtime->stack, buffer[0]); // copy
        break;
    case POP:
        runtime->stack = StackPop(runtime->stack, &buffer[0]);
        break;
    case ADD:
        runtime->stack = StackPop(runtime->stack, &buffer[0]);
        runtime->stack = StackPop(runtime->stack, &buffer[1]);
        runtime->stack = StackPush(runtime->stack, buffer[0] + buffer[1]);
        break;
    case SUB: 
        runtime->stack = StackPop(runtime->stack, &buffer[0]);
        runtime->stack = StackPop(runtime->stack, &buffer[1]);
        runtime->stack = StackPush(runtime->stack, buffer[1] - buffer[1]);
        break;
    case MULT:
        runtime->stack = StackPop(runtime->stack, &buffer[0]);
        runtime->stack = StackPop(runtime->stack, &buffer[1]);
        runtime->stack = StackPush(runtime->stack, buffer[0] * buffer[1]);
        break;
    case DIV:
        runtime->stack = StackPop(runtime->stack, &buffer[0]);
        runtime->stack = StackPop(runtime->stack, &buffer[1]);
        if (buffer[0] != 0) {
            runtime->stack = StackPush(runtime->stack, buffer[1] / buffer[0]);
        } else {
            return 0;
        }
        break;
    case MOD:
        runtime->stack = StackPop(runtime->stack, &buffer[0]);
        runtime->stack = StackPop(runtime->stack, &buffer[1]);
        if (buffer[0] != 0) {
            runtime->stack = StackPush(runtime->stack, buffer[1] % buffer[0]);
        } else {
            return 0;
        }
        break;
    case JUMP:
        ExecuteJump(c, runtime);
        break;
    default:
        break;
    }

    free(buffer);
    free(read);

    return 1;
}