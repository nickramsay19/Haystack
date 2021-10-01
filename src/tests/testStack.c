#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/Stack.h"

#define MAXTESTS 100

int testStack() {
    
    time_t t;
   
    // Intializes random number generator
    srand((unsigned) time(&t));

    int num2Add = 0, isAdding = 0, stackLen = 0;

    Value *buf = (Value*) calloc(1, sizeof(int));
    Stack s = StackNew(5);
    printf("Pushing 5.\n");

    for (int i = 0; i < MAXTESTS; i++) {
        //printf("got here\n");
        num2Add = rand() % 1000;
        isAdding = (int) (rand() % 100) / 50; // number between 0 and 1
        //printf("got here 2 %d %d\n", num2Add, isAdding);

        stackLen = StackLen(s);

        if (isAdding || stackLen <= 0) {
            //printf("A\n");
            
            s = StackPush(s, num2Add);
            printf("Pushing %d - Len: %d -> %d\n", num2Add, stackLen, StackLen(s));
        } else {
            s = StackPop(s, buf);
            printf("Popped %d - Len: %d -> %d\n", *buf, stackLen, StackLen(s));
        }
    }

    return 0;
}