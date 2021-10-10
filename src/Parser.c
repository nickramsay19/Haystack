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

    // assume the cond_type is none
    runtime->cond_type = COND_NONE;

    // strip comments
    int count = 0;
    char *stmtcpy = StripComments(stmt, &count);

    // check that the stmt isn't empty
    if (count == 0) {
        runtime->executing = false;
        return NONE;
    }
    
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
        Command c = ParsePush(&tokens[1], runtime);
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
            
            runtime->error_type = ERROR_COND;
            return NONE;
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
            return NONE;
        } else {
            //runtime->then = true;
            runtime->cond_type = COND_THEN;
            return Parse((&tokens[1]), runtime);
        }
    } else if(strcmp(tokens[0], "loop") == 0) {
        return LOOP;
    } else if(strcmp(tokens[0], "jump") == 0) {
        return JUMP;
    } else if(strcmp(tokens[0], "macro") == 0) {
        return MACRO;
    } else if(strcmp(tokens[0], "done") == 0) {
        return DONE;
    } else {
        runtime->error_type = ERROR_SYNTAX;
        return NONE;
    }
}

// utility methods
Command ParsePush(char **tokens, Runtime runtime) {
    
    // declare integer to be pushed to stack
    int j;

    // check if char specified
    if (tokens[0][0] == '\'') {
        
        if (strcmp(tokens[0], "'\\n'") == 0) {
            j = 10; // newline in ASCII

        } else if (strcmp(tokens[0], "'\\t'") == 0) {
            j = 9; // tab in ASCII

        } else if (tokens[0][2] == '\'') {
            j = (int) tokens[0][1];

        } else {
            runtime->error_type = ERROR_PUSH_INVALID;
            return PUSH;
        }

    // otherwise, we assume an integer literal has been passed
    } else {
        
        j = atoi(tokens[0]);
    }

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

    // store index of words char being read in i, index of wordscpy being written in k
    int i = 0, k = 0;

    // find the first non-whitespace
    for (; i < words_len; i++) {
        if (words[i] != ' ' && words[i] != '\t' && words[i] != '\n') {
            break;
        }
    }

    // add all chars up to the first semicolon
    for (;i < words_len; i++) {
        if (words[i] == ';') {
            break;
        }

        // don't add newlines
        if (words[i] != '\n') {
            wordscpy[k++] = words[i];
        }
    }

    *count = k;

    // shrink wordscpy
    wordscpy = realloc(wordscpy, i * sizeof(char));

    return wordscpy;
}