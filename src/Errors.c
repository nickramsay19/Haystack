#include <stdio.h>
#include <stdlib.h>

#include "include/Runtime.h"
#include "include/Errors.h"

int HandleError(Runtime r) {
    switch (r->error_type) {
    case ERROR_SYNTAX:
        printf("Error: Syntax error (line: %d).\n", r->line_num);
        return 0;
    case ERROR_COND:
        printf("Error: Conditional error (line: %d).\nNote: Is there an incorrectly structured conditional sequence?\n", r->line_num);
        return 0;
    case ERROR_DIVISION:
        printf("Error: Division by zero error (line: %d).\n", r->line_num);
        break;
    case ERROR_COND_LOOP:
        printf("Error: Conditional loop error (line: %d).\nNote: Loops cannot be declared conditionally.", r->line_num);
        return 0;
    case ERROR_PUSH_INVALID:
        printf("Error: Invalid push statement (line: %d).\nNote: Push statements must be of the form \"push <X>\", where <X> is an integer (e.g. \"push 24\") or a character in quotes (e.g. \"push 'a'\").", r->line_num);
        return 0;
    default:
        printf("Error: Unknown error (line: %d).\n", r->line_num);
        return 0;
    }
}

