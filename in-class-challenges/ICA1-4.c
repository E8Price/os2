#include <stdio.h>
#include <pthread.h>

#define ARRAY_SIZE 100000
#define NUM_THREADS 10

int a[ARRAY_SIZE] = {0};

void* increment_array(void* arg) {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        a[i]++; 
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int i;

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment_array, NULL);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    for (i = 0; i < ARRAY_SIZE; i++) {
        if (a[i] != 10) {
            printf("a[%d] = %d, expected 10\n", i, a[i]);
        }
    }

    return 0;
}

/*
Explanation:
We have a global array with 100,000 elements starting at 0
We launch 10 threads. Each thread loops through the array and adds 1 to each element
Ideally every element should end up as 10
However, the "a[i]++" operation is not safe when many threads use it at the same time
This race condition causes some increments to be lost, so some elements may be less than 10
*/
