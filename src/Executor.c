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
        printf("Error: Conditional error (line: %d) in execution.\nNote: Is there an incorrectly structured conditional sequence?\n", runtime->line_num);
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
    case JUMP:
        Execute(c, runtime);
        break;
    case LOOP:
        ExecuteLoop(c, runtime);
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

int ExecuteLoop(Command c, Runtime runtime) {
    // edit runtime
    runtime->loop_depth++;
    runtime->loop_reference[runtime->loop_depth] = runtime->line_num;

    return 1;
}

int Execute(Command c, Runtime runtime) {

    int *buffer = calloc(BUFFER_SIZE, sizeof(int));
    char *read = malloc(sizeof(char));

    if (runtime->cond) {

        // check for conditional loop header
        if (c == LOOP) {
            runtime->executing = false;
            runtime->error = true;
            return 0;
        }

        // first we check the stack condition
        int pass = 0;

        switch (runtime->cond_type) {
        case COND_MAYBE:
            
            // pop the stack condition
            runtime->stack = StackPop(runtime->stack, &pass);

            if (pass) {
                runtime->cond_carry = true;
                runtime->cond_triggered = true;
                runtime->executing = true;
            } else {
                runtime->cond_carry = false;
                runtime->cond_triggered = false;
                runtime->executing = false;
            }
            break;
        case COND_OR:

            // pop the stack condition
            runtime->stack = StackPop(runtime->stack, &pass);

            // check the previous conditions exclusion rule before checking stack condition
            if (runtime->cond_triggered) {

                // set the carry to ensure no further then's are called
                runtime->cond_carry = false;

                // push back the condition, since we needn't have checked it
                runtime->stack = StackPop(runtime->stack, &pass);

                // cancel execution, conditions didn't trigger
                runtime->executing = false;
            } else if (pass) {
                runtime->cond_carry = true;
                runtime->cond_triggered = true;
                runtime->executing = true;
            } else {
                runtime->cond_carry = false;
                runtime->cond_triggered = false;
                runtime->executing = false;
            }
            break;
        case COND_THEN:
            if (!runtime->cond_carry) {
                // do not execute, previous condition did not pass
                runtime->executing = false;
            } else {
                runtime->cond_carry = true;
                runtime->cond_triggered = true;
                runtime->executing = true;
            }
            break;
        default:
            // error
            break;
        }

        // Check for failed jumps
        if (!runtime->executing && c == JUMP) {
            runtime->loop_depth--;
        } 

        // cancel non-executing commands
        if (!runtime->executing) {

            // increment line number before ending execution
            //runtime->line_num++;

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
        // set runtime line number
        runtime->line_num = runtime->loop_reference[runtime->loop_depth];

        // since we are jumping, we need to reset cond flags to default
        runtime->cond = false;
        runtime->cond_carry = false;
        runtime->cond_triggered = false;
        runtime->cond_type = COND_NONE;

        break;
    default:
        break;
    }

    free(buffer);
    free(read);

    return 1;
}