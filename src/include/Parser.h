#ifndef PARSER_H
#define PARSER_H

#include "Runtime.h"

enum CommandEnum {
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
    MACRO,
    DONE,
};
typedef enum CommandEnum Command;

Command ParseStatement(char* stmt, Runtime runtime);

#endif