#include <pthread.h>
#include <stdio.h>

typedef struct {
    char *child_hello;
    char *child_goodbye;
} ThreadMessages;

void *run(void *arg)
{
    ThreadMessages *msgs = (ThreadMessages *)arg;
    printf("%s\n", msgs->child_hello);
    printf("%s\n", msgs->child_goodbye);
    return NULL;
}

int main(){
    pthread_t thread_id;
    ThreadMessages msg;

    msg.child_hello = "child: custom hello!";
    msg.child_goodbye = "child: custom goodbye!";
    
    printf("parent: hello!\n");

    if (pthread_create(&thread_id, NULL, run, &msg)) {
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
