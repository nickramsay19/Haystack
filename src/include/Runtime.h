#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdbool.h>
#include "Stack.h"

#define MAX_PAYLOAD 2

struct RuntimeRep {
    bool cond; // conditional sequence flag
    bool cond_carry;
    int loop; // loop depth
    int *payload;

    Stack stack;
};

typedef struct RuntimeRep *Runtime;

Runtime RuntimeNew();

void RuntimeFree(Runtime r);

#endif