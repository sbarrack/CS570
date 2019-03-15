#include <stdio.h>
#include <pthread.h>
#include "worker.h"

#define NUM_THREADS 5

pthread_t thread[NUM_THREADS]; // the 5 threads
pthread_attr_t ptattr;
int i;

int main() {
    pthread_attr_init(&ptattr);
    for (i = 0; i < NUM_THREADS; i++) { // loop that spawns the threads
        pthread_create(&thread[i], &ptattr, (void *) &worker, &i); // the thread
        pthread_join(thread[i], NULL); // wait for that thread
    }
    printf("work complete\n");
}
