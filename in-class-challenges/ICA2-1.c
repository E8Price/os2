#include <stdio.h>
#include <pthread.h>

#define ELEMENTS 10000
#define THREADS 4

int a[ELEMENTS] = {0};
pthread_mutex_t lock_mutex = PTHREAD_MUTEX_INITIALIZER;

void *run(void *arg) {
    (void)arg;
#ifdef LOCK_INSIDE_LOOP
    for (int i = 0; i < ELEMENTS; i++) {
        pthread_mutex_lock(&lock_mutex);
        a[i]++;
        pthread_mutex_unlock(&lock_mutex);
    }
#else
    pthread_mutex_lock(&lock_mutex);
    for (int i = 0; i < ELEMENTS; i++) {
        a[i]++;
    }
    pthread_mutex_unlock(&lock_mutex);
#endif
    return NULL;
}

int main(void) {
    pthread_t thread[THREADS];

    for (int i = 0; i < THREADS; i++)
        pthread_create(&thread[i], NULL, run, NULL);

    for (int i = 0; i < THREADS; i++)
        pthread_join(thread[i], NULL);

    int expected = THREADS;
    for (int i = 0; i < ELEMENTS; i++)
        if (a[i] != expected)
            printf("a[%d] = %d, expected %d\n", i, a[i], expected);

    return 0;
}

/*
- Locking inside the loop means we lock and unlock every time we add 1, this takes extra time
- Locking the whole loop means we lock once and unlock once per thread, which is faster
The extra time comes from doing many lock and unlock operations
*/
