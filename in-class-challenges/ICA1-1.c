#include <pthread.h>
#include <stdio.h>

void *run(void *arg)
{
    printf("child: hello!\n");
    printf("child: goodbye\n");
    return NULL;
}

int main(){
    pthread_t thread_id;
    printf("parent: hello!\n");

    if (pthread_create(&thread_id, NULL, *run, NULL)) {
        fprintf(stderr, "Error\n");
        return 1;
    }

    if (pthread_join(thread_id, NULL)) {
        fprintf(stderr, "Error\n");
        return 1;
    }

    printf("parent: goodbye\n");

    return 0;
}


