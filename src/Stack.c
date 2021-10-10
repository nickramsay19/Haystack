#include <stdio.h>
#include <stdlib.h>

#include "include/Stack.h"

void printStack(Stack s);

Stack StackNew(Value value) {

    Stack s = (Stack) malloc(sizeof(struct StackRep));
    s->value = value;
    s->next = NULL;

    return s;
}

void StackFree(Stack s) {
    free(s);
}

Stack StackPop(Stack s, Value* v) {

    if (s == NULL) return NULL;

    // place value at the top of the stack into the provided value buffer
    *v = s->value;

    // delete top entry
    Stack s1 = s->next;
    free(s);

    return s1;
}

Stack StackPush(Stack s, Value v) {
    //printf("printing stack: ");
    //printStack(s);
    Stack s1 = StackNew(v);
    s1->next = s;
    
    return s1;
}

int StackLen(Stack s) {
    int len = 0;
    while (s->next != NULL) {
        s = s->next;
        len++;
    }

    return len;
}

void StackPrint(Stack s) {
    Stack t = s;
    printf("[%d", s->value);
    while (t && t->next) {
        t = t->next;
        printf(", %d", t->value);
       
    }
    printf("]\n");
}