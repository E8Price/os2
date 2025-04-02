#include <stdio.h>
#include <pthread.h>

void* thread_function(void *arg) {
    printf("running\n");
    return NULL;
}

int main() {
    pthread_t threads[3];
    int i;

    for (i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, NULL)) {
            return 1;
        }
    }

    for (i = 0; i < 3; i++) {
        if (pthread_join(threads[i], NULL)) {
            return 1;
        }
    }

    printf("main thread  complete\n");

    return 0;
}
