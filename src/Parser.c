#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "include/Stack.h"
#include "include/Parser.h"
#include "include/Runtime.h"

#define MAX_STRING 1000
#define MAX_WORD 50

// utility method headers
Command Parse(char **tokens, Runtime runtime);
Command ParsePush(char **tokens, Runtime runtime);
Command ParseConditional(char **tokens, Runtime runtime, ConditionType type);
Command ParseThen(char **tokens, Runtime runtime);
char **Split(char *words, int *count);
char *StripComments(char *words, int *count);

void debugp(float x) {
    //printf("%f\n", x);
}

Command ParseStatement(char* stmt, Runtime runtime) {

    // set the executing flag
    // assume we execute the line
    runtime->executing = true;

    // assume that the command isn't a THEN_ command
    runtime->then = false;

    // strip comments
    int count = 0;
    char *stmtcpy = StripComments(stmt, &count);
    

    // convert statement to array of word tokens
    int words = 0;
    char **stmtwords = Split(stmtcpy, &words);

    debugp(1);

    return Parse(stmtwords, runtime);
}

Command Parse(char **tokens, Runtime runtime) {

    if(strcmp(tokens[0], "read") == 0) {
        return READ;
    } else if(strcmp(tokens[0], "print") == 0) {
        return PRINT;
    } else if(strcmp(tokens[0], "push") == 0) {
        Command c = ParsePush(tokens, runtime);
        return c;
    } else if(strcmp(tokens[0], "copy") == 0) {
        return COPY;
    } else if(strcmp(tokens[0], "pop") == 0) {
        return POP;
    } else if(strcmp(tokens[0], "add") == 0) {
        return ADD;
    } else if(strcmp(tokens[0], "sub") == 0) {
        return SUB;
    } else if(strcmp(tokens[0], "maybe") == 0) {
        return ParseConditional(tokens, runtime, COND_TYPE_START);
    } else if(strcmp(tokens[0], "or") == 0) {
        return ParseConditional(tokens, runtime, COND_TYPE_CONTINUE);
    } else if(strcmp(tokens[0], "done") == 0) {
        return ParseConditional(tokens, runtime, COND_TYPE_END);
    } else if(strcmp(tokens[0], "then") == 0) {
        return ParseThen(tokens, runtime);
    } else if(strcmp(tokens[0], "loop") == 0) {
        return LOOP;
    } else if(strcmp(tokens[0], "jump") == 0) {
        return JUMP;
    } else if (tokens[0][0] == ';') {
        return NONE; // comment
    } else {
        return SYNTAX_ERROR;
    }
}

// utility methods
Command ParsePush(char **tokens, Runtime runtime) {
    // we assume that the second token in an integer
    // we simply convert tokens[1] to an int

    int j = atoi(tokens[1]);

    // move value into payload
    runtime->payload[0] = j;

    return PUSH;
}

Command ParseConditional(char **tokens, Runtime runtime, ConditionType type) {

    // set the conditional flag
    // and convert command to none command if needed
    switch (type) {
        case COND_TYPE_START:
            // set the cond flag to enter a conditional sequence
            runtime->cond = 1;
            runtime->cond_carry = 0;
            break;
        case COND_TYPE_CONTINUE:
            if (runtime->cond == 0) {
                // set the execution to false
                // but ensure that other flags carry
                runtime->executing = false;
            }
            runtime->cond_carry = 0;
            break;
        case COND_TYPE_END:
            runtime->cond = 0;
            runtime->cond_carry = 0;
            return NONE;
    }

    // we assume that first token is "maybe" or "or"

    // move on to second token
    if(strcmp(tokens[1], "read") == 0) {
        return COND_READ;
    } else if(strcmp(tokens[1], "print") == 0) {
        return COND_PRINT;
    } else if(strcmp(tokens[1], "push") == 0) {
        //printf("read push\n");
        ParsePush(&tokens[1], runtime);
        return COND_PUSH;
    } else if(strcmp(tokens[1], "copy") == 0) {
        return COND_COPY;
    } else if(strcmp(tokens[1], "pop") == 0) {
        return COND_POP;
    } else if(strcmp(tokens[1], "add") == 0) {
        return COND_ADD;
    } else if(strcmp(tokens[1], "jump") == 0) {
        return JUMP;
    } else if(strcmp(tokens[1], "maybe") == 0) {
        return COND_ERROR; // cant have a conditional in a conditional
    } else if (tokens[1][0] == ';') {
        return NONE; // comment
    } else {
        return SYNTAX_ERROR;
    }
}

Command ParseThen(char **tokens, Runtime runtime) {
    if(strcmp(tokens[1], "read") == 0) {
        return THEN_READ;
    } else if(strcmp(tokens[1], "print") == 0) {
        return THEN_PRINT;
    } else if(strcmp(tokens[1], "push") == 0) {
        ParsePush(tokens, runtime); // ignore return value
        return THEN_PUSH;
    } else if(strcmp(tokens[1], "copy") == 0) {
        return THEN_COPY;
    } else if(strcmp(tokens[1], "pop") == 0) {
        return THEN_POP;
    } else if(strcmp(tokens[1], "add") == 0) { // todo: add mult, div, etc
        return THEN_ADD;
    } else if(strcmp(tokens[1], "jump") == 0) {
        runtime->then = true;
        return JUMP;
    } else if(strcmp(tokens[1], "maybe") == 0) {
        return COND_ERROR;
    } else if(strcmp(tokens[1], "or") == 0) {
        return COND_ERROR;
    } else if(strcmp(tokens[1], "done") == 0) {
        return COND_ERROR;
    } else if(strcmp(tokens[1], "then") == 0) {
        return COND_ERROR;
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
        if (words[i] == ' ' || words[i] == '\n' || words[i] == '\t') {
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

char *StripComments(char *words, int *count) {

    int words_len = strlen(words);
    char *wordscpy = calloc(words_len, sizeof(char));

    int i = 0;
    for (i = 0; i < words_len; i++) {
        if (words[i] == ';') {
            break;
        }

        wordscpy[i] = words[i];
    }

    *count = i;

    // shrink wordscpy
    wordscpy = realloc(wordscpy, i * sizeof(char));

    return wordscpy;
}