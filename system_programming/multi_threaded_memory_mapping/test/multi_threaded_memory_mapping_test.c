#include <pthread.h>

void *thread_func(void *arg);

int main() 
{
    pthread_t thread1, thread2;

    /*Creates threads*/
    pthread_create(&thread1, NULL, thread_func, NULL);

    pthread_create(&thread2, NULL, thread_func, NULL);
       
    /*Waits for threads to finish*/
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
