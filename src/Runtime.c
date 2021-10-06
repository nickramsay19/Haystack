#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "include/Stack.h"
#include "include/Runtime.h"

Runtime RuntimeNew() {
    Runtime r = (Runtime) malloc(sizeof(struct RuntimeRep));

    r->stack = StackNew(0);

    r->payload = calloc(MAX_PAYLOAD, sizeof(int));

    r->cond = false;
    r->cond_carry = false;
    r->cond_continue = false;
    r->cond_triggered= false;

    r->loop_depth = 0;
    r->loop_reference = calloc(MAX_LOOP_DEPTH, sizeof(int));

    r->line_num = 1; // start execution on line 1

    r->error = false;

    return r;
}

void RuntimeFree(Runtime r) {

    StackFree(r->stack);

    free(r->payload);

    free(r);
}