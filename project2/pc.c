#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include "eventbuf.h"

struct eventbuf *eb;
sem_t *mutex;
sem_t *items;
sem_t *spaces;

int done = 0;
int num_consumers;

void *producer(void *arg) {
    int id = *(int *)arg;
    int events = ((int *)arg)[1];

    for (int i = 0; i < events; i++) {
        sem_wait(spaces);
        sem_wait(mutex);
        int event = id * 100 + i;
        printf("P%d: adding event %d\n", id, event);
        eventbuf_add(eb, event);
        sem_post(mutex);
        sem_post(items);
    }

    printf("P%d: exiting\n", id);
    return NULL;
}

void *consumer(void *arg) {
    int id = *(int *)arg;

    while (1) {
        sem_wait(items);
        sem_wait(mutex);

        if (done && eventbuf_empty(eb)) {
            sem_post(mutex);
            break;
        }

        if (!eventbuf_empty(eb)) {
            int event = eventbuf_get(eb);
            printf("C%d: got event %d\n", id, event);
        }

        sem_post(mutex);
        sem_post(spaces);
    }

    printf("C%d: exiting\n", id);
    return NULL;
}

sem_t *sem_open_temp(const char *name, int value) {
    sem_t *sem = sem_open(name, O_CREAT, 0600, value);
    if (sem == SEM_FAILED) return SEM_FAILED;
    sem_unlink(name);
    return sem;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s producers consumers events max_outstanding\n", argv[0]);
        exit(1);
    }

    int num_producers = atoi(argv[1]);
    num_consumers = atoi(argv[2]);
    int events = atoi(argv[3]);
    int max_out = atoi(argv[4]);

    eb = eventbuf_create();
    mutex = sem_open_temp("/pc-mutex", 1);
    items = sem_open_temp("/pc-items", 0);
    spaces = sem_open_temp("/pc-spaces", max_out);

    pthread_t producers[num_producers], consumers[num_consumers];
    int prod_args[num_producers][2];
    int cons_args[num_consumers];

    for (int i = 0; i < num_producers; i++) {
        prod_args[i][0] = i;
        prod_args[i][1] = events;
        pthread_create(&producers[i], NULL, producer, prod_args[i]);
    }

    for (int i = 0; i < num_consumers; i++) {
        cons_args[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &cons_args[i]);
    }

    for (int i = 0; i < num_producers; i++) {
        pthread_join(producers[i], NULL);
    }

    sem_wait(mutex);
    done = 1;
    sem_post(mutex);
    for (int i = 0; i < num_consumers; i++) {
        sem_post(items);
    }

    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    eventbuf_free(eb);
    sem_close(mutex);
    sem_close(items);
    sem_close(spaces);

    return 0;
}
