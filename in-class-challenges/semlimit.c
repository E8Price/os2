#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define THREAD_COUNT 15

sem_t *sem_open_temp(const char *name, unsigned int value)
{
    sem_t *sem;

    if ((sem = sem_open(name, O_CREAT, 0600, value)) == SEM_FAILED)
        return SEM_FAILED;

    if (sem_unlink(name) == -1) {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}

sem_t *sem; 

void *run(void *arg)
{
    int *id = arg;

    sem_wait(sem); 

    printf("Thread %02d: running\n", *id);

    sleep(1);  

    printf("Thread %02d: exiting\n", *id);

    sem_post(sem); 

    return NULL;
}


int main(void)
{
    sem = sem_open_temp("limit", 3);

    // Do not modify code between these marks vvv
    pthread_t runners[THREAD_COUNT];
    int runner_id[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++) {
        runner_id[i] = i;
        pthread_create(runners + i, NULL, run, runner_id + i);  // Run, runner!
    }

    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_join(runners[i], NULL);
    // Do not modify code between these marks ^^^

}
