#include <stdio.h>
#include <math.h>
#include "worker.h"

// thread entry point
void worker(int *num) {
    printf("%.0f\n", pow(*num, 2)); // print without decimals
}
