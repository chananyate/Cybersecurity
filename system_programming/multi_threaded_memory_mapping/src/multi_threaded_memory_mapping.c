#include <stdio.h>
#include <stdlib.h>   /*malloc*/
#include <pthread.h>  /*pthread_self(), pthread_exit()*/

/*Global variable*/
int global_var = 42;

/*Thread function to print memory locations*/
void *thread_func(void *arg) 
{
    size_t thread_id = pthread_self();
    /*Local variable (on thread stack)*/
    int local_var = 10;

    /*Dynamic memory allocation (on heap)*/
    int *heap_var = malloc(sizeof(int));
    if (NULL == heap_var) 
    {
        perror("Failed to allocate memory");
        pthread_exit(NULL);  /*Terminates the calling thread.*/
    }
    *heap_var = 20;

    /*printf("Thread ID: %lu\n", thread_id);  /Returns the thread ID of the calling thread/
    /*printf("Address of local variable (thread stack): %p\n", (void *)&local_var);
    printf("Address of dynamic variable (heap): %p\n", (void *)heap_var);
    printf("Address of global variable: %p\n", (void *)&global_var);*/

    free(heap_var); 
    pthread_exit(NULL);
}