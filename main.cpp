#include <cstdio>
#include "cross_array.hpp"

int main() {
    // Minimal integration to ensure build; no I/O required by problem
    // but OJ expects an executable. We'll do a small self-check.
    sjtu::CrossArray ca(2);
    int a[3] = {1,2,3};
    ca.InsertArrays(a, 3);
    int b[2] = {4,5};
    ca.InsertArrays(b, 2);
    (void)ca; // suppress unused warnings
    return 0;
}
