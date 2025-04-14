#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int seat_count;
int broker_count;
int *seat_taken;  
int transaction_count;

int seat_taken_count = 0;

pthread_mutex_t seat_mutex = PTHREAD_MUTEX_INITIALIZER;

int reserve_seat(int n)
{
    pthread_mutex_lock(&seat_mutex);

    if (seat_taken[n]) {
        pthread_mutex_unlock(&seat_mutex);
        return -1;
    }

    seat_taken[n] = 1;
    seat_taken_count++;

    pthread_mutex_unlock(&seat_mutex);
    return 0;
}

int free_seat(int n)
{
    pthread_mutex_lock(&seat_mutex);

    if (!seat_taken[n]) {
        pthread_mutex_unlock(&seat_mutex);
        return -1;
    }

    seat_taken[n] = 0;
    seat_taken_count--;

    pthread_mutex_unlock(&seat_mutex);
    return 0;
}

int is_free(int n) {
    pthread_mutex_lock(&seat_mutex);
    int result = seat_taken[n] == 0;
    pthread_mutex_unlock(&seat_mutex);
    return result;
}

int verify_seat_count(void) {
    pthread_mutex_lock(&seat_mutex);

    int count = 0;
    for (int i = 0; i < seat_count; i++)
        if (seat_taken[i])
            count++;

    int valid = (count == seat_taken_count);

    pthread_mutex_unlock(&seat_mutex);
    return valid;
}

// ------------------- DO NOT MODIFY PAST THIS LINE -------------------

void *seat_broker(void *arg)
{
    int *id = arg;

    for (int i = 0; i < transaction_count; i++) {
        int seat = rand() % seat_count;
        if (rand() & 1) {
            // buy a random seat
            reserve_seat(seat);

        } else {
            // sell a random seat
            free_seat(seat);
        }

        if (!verify_seat_count()) {
            printf("Broker %d: the seat count seems to be off! " \
                   "I quit!\n", *id);
            return NULL;
        }
    }

    printf("Broker %d: That all seemed to work very well.\n", *id);

    return NULL;
}

int main(int argc, char *argv[])
{
    // Parse command line
    if (argc != 4) {
        fprintf(stderr, "usage: reservations seat_count broker_count xaction_count\n");
        exit(1);
    }

    seat_count = atoi(argv[1]);
    broker_count = atoi(argv[2]);
    transaction_count = atoi(argv[3]);

    // Allocate the seat-taken array
    seat_taken = calloc(seat_count, sizeof *seat_taken);

    // Allocate thread handle array for all brokers
    pthread_t *thread = calloc(broker_count, sizeof *thread);

    // Allocate thread ID array for all brokers
    int *thread_id = calloc(broker_count, sizeof *thread_id);

    srand(time(NULL) + getpid());
    
    // Launch all brokers
    for (int i = 0; i < broker_count; i++) {
        thread_id[i] = i;
        pthread_create(thread + i, NULL, seat_broker, thread_id + i);
    }

    // Wait for all brokers to complete
    for (int i = 0; i < broker_count; i++)
        pthread_join(thread[i], NULL);

    // Free everything
    free(thread_id);
    free(thread);
    free(seat_taken);
}

