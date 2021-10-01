#ifndef RUNTIME_H
#define RUNTIME_H

#include <stdbool.h>

struct RuntimeRep {
    bool cond; // conditional sequence
    int loop; // loop depth
}

#endif