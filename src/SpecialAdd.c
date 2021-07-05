#include "include/Add.h"

int SpecialAdd(int a, int b) {
    return Add(Add(a, b), a);
}