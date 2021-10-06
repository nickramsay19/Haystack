#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdbool.h>
#include "Stack.h"

#define MAX_PAYLOAD 2
#define MAX_LOOP_DEPTH 10

struct RuntimeRep {
    int *payload; // payload carries parsed tokens to the executor

    bool cond; // conditional sequence flag
    bool cond_continue; // 

    bool cond_triggered;
    bool cond_carry; // carry to a then
    
    int loop_depth; // loop depth
    int *loop_reference;

    int line_num;

    bool executing;

    bool then;

    bool error;

    Stack stack;
};

typedef struct RuntimeRep *Runtime;

Runtime RuntimeNew();

void RuntimeFree(Runtime r);

#endif