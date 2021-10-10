#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdbool.h>

#include "Stack.h"

#define MAX_PAYLOAD 2
#define MAX_LOOP_DEPTH 10
#define MAX_MACRO_NAME 25

enum ConditionTypeEnum {
    COND_NONE,
    COND_MAYBE,
    COND_OR,
    COND_THEN
};
typedef enum ConditionTypeEnum ConditionType;

enum ErrorTypeEnum {
    ERROR_NONE,
    ERROR_SYNTAX,
    ERROR_COND,
    ERROR_DIVISION,
    ERROR_COND_LOOP,
    ERROR_PUSH_INVALID,
    ERROR_MACRO_NO_NAME,
};
typedef enum ErrorTypeEnum ErrorType;

struct RuntimeRep {
    int *payload; // payload carries parsed tokens to the executor

    bool cond; // conditional sequence flag, true if currently in a cond sequence
    bool cond_triggered; // true if the current cond seq has had a true triggered
    bool cond_carry; // carry to a then
    ConditionType cond_type;
    
    int loop_depth; // loop depth
    int *loop_reference;

    bool macro;
    char *macro_name;

    int line_num;

    bool executing;

    bool then;

    ErrorType error_type;

    Stack stack;
};
typedef struct RuntimeRep *Runtime;

Runtime RuntimeNew();

void RuntimeFree(Runtime r);

#endif