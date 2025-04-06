#include <stdio.h>
#include <pthread.h>

void* thread_function(void *arg) {
    char *thread_name = (char *) arg;
    for (int i = 0; i < 5; i++) {
        printf("%s: %d\n", thread_name, i);
    }
    return NULL;
}

int main(void) {
    pthread_t thread1, thread2;

    printf("Launching threadss\n");

    pthread_create(&thread1, NULL, thread_function, "thread 1");
    pthread_create(&thread2, NULL, thread_function, "thread 2");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Threads complete\n");
    return 0;
}
