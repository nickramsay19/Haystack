#ifndef PARSER_H
#define PARSER_H

#include "Runtime.h"

enum CommandEnum {
    COND_ERROR = -3,
    TYPE_ERROR = -2,
    SYNTAX_ERROR = -1,
    NONE,
    READ,
    PRINT,
    PUSH,
    COPY,
    POP,
    ADD,
    SUB,
    MULT,
    DIV,
    MOD,
    JUMP,
    LOOP,
};
typedef enum CommandEnum Command;

Command ParseStatement(char* stmt, Runtime runtime);

#endif