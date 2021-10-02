#ifndef PARSER_H
#define PARSER_H

#include "Runtime.h"

enum CommandEnum {
    COND_ERROR = -3,
    TYPE_ERROR = -2,
    SYNTAX_ERROR = -1,
    NONE,
    READ, // we only have integers for now
    PRINT,
    PUSH,
    COPY,
    POP,
    ADD,
    SUB,
    MULT,
    DIV,
    MOD,
    BREAK, 
    COND_READ,
    COND_PRINT,
    COND_PUSH,
    COND_COPY,
    COND_POP,
    COND_ADD,
    COND_SUB,
    COND_MULT,
    COND_DIV,
    COND_MOD,
    COND_BREAK,
    THEN_READ,
    THEN_PRINT,
    THEN_PUSH,
    THEN_COPY,
    THEN_POP,
    THEN_ADD,
    THEN_SUB,
    THEN_MULT,
    THEN_DIV,
    THEN_MOD,
    THEN_BREAK
};
typedef enum CommandEnum Command;

enum ConditionTypeEnum {
    COND_TYPE_START,
    COND_TYPE_CONTINUE,
    COND_TYPE_END
};
typedef enum ConditionTypeEnum ConditionType;

Command ParseStatement(char* stmt, Runtime runtime);

#endif