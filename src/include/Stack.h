#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

typedef int Value;

typedef struct StackRep *Stack;

struct StackRep {
    Value value;
    Stack next;
};

Stack StackNew(Value value);

void StackFree(Stack s);

Stack StackPop(Stack s, Value *v);

Stack StackPush(Stack s, Value v);

int StackLen(Stack s);

#endif