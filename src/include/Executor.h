#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "Parser.h"
#include "Stack.h"

int ExecuteConditionally(Command c, Stack *s, int *condflag);

int Execute(Command c, Stack *s);

#endif