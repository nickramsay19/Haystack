#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "include/Stack.h"
#include "include/Parser.h"
#include "include/Runtime.h"
#include "include/Errors.h"

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

    // assume that the command isn't a then command
    //runtime->then = false;

    // assume the cond_type is none
    runtime->cond_type = COND_NONE;

    // strip comments
    int count = 0;
    char *stmtcpy = StripComments(stmt, &count);
    
    // convert statement to array of word tokens
    int words = 0;
    char **tokens = Split(stmtcpy, &words);

    // first check if this statement is not a conditional
    // if not, set the cond flag to false, ending a conditional sequence
    if (strcmp(tokens[0], "maybe") != 0 && strcmp(tokens[0], "or") != 0 && strcmp(tokens[0], "then") != 0){
        // set the cond flag to false
        runtime->cond = false;
        runtime->cond_triggered = false;
        runtime->cond_carry = false;
    }

    return Parse(tokens, runtime);
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
    } else if(strcmp(tokens[0], "mult") == 0) {
        return MULT;
    } else if(strcmp(tokens[0], "div") == 0) {
        return DIV;
    } else if(strcmp(tokens[0], "mod") == 0) {
        return MOD;
    } else if(strcmp(tokens[0], "maybe") == 0) {
        // set the cond flag to enter a conditional sequence
        runtime->cond = true;
        runtime->cond_type = COND_MAYBE;
        return Parse((&tokens[1]), runtime);
    } else if(strcmp(tokens[0], "or") == 0) {
        if (!runtime->cond) {
            // set the execution to false
            runtime->executing = false;
            return COND_ERROR;
        } else {
            //runtime->cond_carry = 0;
            runtime->cond_type = COND_OR;
            return Parse((&tokens[1]), runtime);
        }
    } else if(strcmp(tokens[0], "then") == 0) {
         if (!runtime->cond) {
            // set the execution to false
            runtime->executing = false;
            runtime->error_type = ERROR_COND;
            return COND_ERROR;
        } else {
            //runtime->then = true;
            runtime->cond_type = COND_THEN;
            return Parse((&tokens[1]), runtime);
        }
    } else if(strcmp(tokens[0], "loop") == 0) {
        return LOOP;
    } else if(strcmp(tokens[0], "jump") == 0) {
        return JUMP;
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