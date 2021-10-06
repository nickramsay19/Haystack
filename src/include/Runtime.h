#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdbool.h>
#include "Stack.h"

#define MAX_PAYLOAD 2
#define MAX_LOOP_DEPTH 10

enum ConditionTypeEnum {
    COND_MAYBE,
    COND_OR,
    COND_THEN,
    COND_NONE,
};
typedef enum ConditionTypeEnum ConditionType;

struct RuntimeRep {
    int *payload; // payload carries parsed tokens to the executor

    bool cond; // conditional sequence flag, true if currently in a cond sequence
    bool cond_triggered; // true if the current cond seq has had a true triggered
    bool cond_carry; // carry to a then
    ConditionType cond_type;
    
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