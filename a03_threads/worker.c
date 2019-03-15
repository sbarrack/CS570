#include <math.h>
#include <stdio.h>
#include "worker.h"

#define SQUARE 2 // pow(x, 2) will give us x^2

// thread entry point
void worker(void *num) {
    // worker() expects an int *, need a double for pow()
    double sq = pow((double) *(int *)num, SQUARE);
    printf("%.0f\n", sq); // print without decimals
}
