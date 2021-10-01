#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "include/Stack.h"
#include "include/Parser.h"

#define MAX_STRING 1000
#define MAX_WORD 50

// utility method headers
Command Parse(char **tokens, int *res);
Command ParsePush(char **tokens, int *res);
Command ParseConditional(char **tokens, int *res);
char **Split(char *words, int *count);

Command ParseStatement(char* stmt, int *res) {

    // convert statement to array of word tokens
    int words = 0;
    char **stmtwords = Split(stmt, &words);

    return Parse(stmtwords, res);
}

Command Parse(char **tokens, int *res) {

    if(strcmp(tokens[0], "read") == 0) {
        return READ;
    } else if(strcmp(tokens[0], "print") == 0) {
        return PRINT;
    } else if(strcmp(tokens[0], "push") == 0) {
        Command c = ParsePush(tokens, res);
        return c;
    } else if(strcmp(tokens[0], "copy") == 0) {
        return COPY;
    } else if(strcmp(tokens[0], "pop") == 0) {
        //return SYNTAX_ERROR; // if I remove pop, replace with this
        return POP;
    } else if(strcmp(tokens[0], "add") == 0) {
        return ADD;
    } else if(strcmp(tokens[0], "maybe") == 0) {
        return ParseConditional(tokens, res);
    } else {
        return SYNTAX_ERROR;
    }
}

// utility methods
Command ParsePush(char **tokens, int *res) {
    // we assume that the second token in an integer
    // we simply convert tokens[1] to an int

    int j = atoi(tokens[1]);
    *res = j;

    return PUSH;
}

Command ParseConditional(char **tokens, int *res) {

    // we assume that first token is "maybe"
    // move on to second token

    if(strcmp(tokens[1], "read") == 0) {
        return COND_READ;
    } else if(strcmp(tokens[1], "print") == 0) {
        return COND_PRINT;
    } else if(strcmp(tokens[1], "push") == 0) {
        ParsePush(tokens, res);
        return COND_PUSH;
    } else if(strcmp(tokens[1], "copy") == 0) {
        return COND_COPY;
    } else if(strcmp(tokens[1], "pop") == 0) {
        //return SYNTAX_ERROR; // if I remove pop, replace with this
        return COND_POP;
    } else if(strcmp(tokens[1], "add") == 0) {
        return COND_ADD;
    } else if(strcmp(tokens[1], "maybe") == 0) {
        return COND_ERROR; // cant have a conditional in a conditional
    } else {
        return SYNTAX_ERROR;
    }
}

char **Split(char *words, int *count) {

    char **arr = calloc(MAX_STRING / MAX_WORD, sizeof(char*));
    arr[0] = calloc(MAX_WORD, sizeof(char));

    int w = 0;
    int wi = 0;
    for (int i = 0; i < strlen(words); i++) {
        if (words[i] == ' ' || words[i] == '\n') {
            w++;
            wi = 0;
            arr[w] = calloc(MAX_WORD, sizeof(char));
        } else {
            arr[w][wi++] = words[i];
        }
    }

    *count = w;

    return arr;
}