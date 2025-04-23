#include <stdio.h>
#include <stdlib.h>

// vvvvvvvvvv Don't change between marks vvvvvvvvvv

// RUNF is a pointer to a function that takes void as an argument
// and returns void.

typedef void (*RUNF)(void);

// Linked list of functions to run
struct task {
    RUNF run_function;
    struct task *next;
};

// Keep track of head and tail
struct task_list {
    struct task *head, *tail;
};

struct task_list task_list = { .head=NULL, .tail=NULL };

// Append a run function to the tail of the list
void append_task(RUNF run_function)
{
    struct task *n = malloc(sizeof *n);
    n->run_function = run_function;
    n->next = NULL;

    if (task_list.head == NULL) {
        task_list.head = task_list.tail = n;
    } else {
        task_list.tail->next = n;
        task_list.tail = n;
    }
}

// Pop a run function from the head of the list
RUNF remove_task(void)
{
    if (task_list.head == NULL)
        return NULL;

    RUNF runf = task_list.head->run_function;
    struct task *n = task_list.head;
    task_list.head = task_list.head->next;
    free(n);

    return runf;
}

// ^^^^^^^^^^ Don't change between marks ^^^^^^^^^^

void run1(void)
{
    static int i = 0;
    if (i < 5) {
        printf("task 1: %d\n", i);
        i++;
        append_task(run1); 
    }
}

void run2(void)
{
    static int i = 0;
    if (i < 5) {
        printf("task 2: %d\n", i);
        i++;
        append_task(run2);  
    }
}

// Don't change main!
int main(void)
{
    append_task(run1);
    append_task(run2);

    while (task_list.head) {
        RUNF runner = remove_task();
        runner();
    }
}

