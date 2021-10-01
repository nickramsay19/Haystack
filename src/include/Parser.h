#ifndef PARSER_H
#define PARSER_H

enum CommandEnum {
    COND_ERROR = -3,
    TYPE_ERROR = -2,
    SYNTAX_ERROR = -1,
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

};
typedef enum CommandEnum Command;

Command ParseStatement(char* stmt, int *res);

#endif