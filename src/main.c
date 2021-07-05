#include <stdio.h>
#include <stdlib.h>

#include "include/SpecialAdd.h"

int main(int argc, char** argv) {
    if (argc > 2) {
        int a = atoi(argv[1]), b = atoi(argv[2]);

        printf("SpecialAdd(%d, %d) = %d\n", a, b, SpecialAdd(a,b));
    }

    return 0;
}