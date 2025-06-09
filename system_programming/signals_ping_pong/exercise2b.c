/*pong*/
#include <stdio.h>   /*printf*/
#include <signal.h>  /*signal, kill*/
#include <stdlib.h>  /*atoi*/
#include <unistd.h>  /*getpid, pause*/

/*Flag to indicate if signal was received. 
  The type sig_atomic_t guarantees that accesses to this variable are atomic, meaning 
  it completes without any possibility of being interrupted or affected by other processes or threads, 
  making it safe for use in signal handlers.*/
volatile sig_atomic_t signal_received = 0;

void signal_handler(int signo)   
{
    if (signo == SIGUSR1) 
        printf("Received SIGUSR1\n");
    else if (signo == SIGUSR2) 
        printf("Received SIGUSR2\n");
    
    signal_received = 1;
}

int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        fprintf(stderr, "Usage: %s <parent PID>\n", argv[0]);
        return 1;
    }

    pid_t parent_pid = atoi(argv[1]);
    
    /*Sets up the signal handler for SIGUSR1*/
    signal(SIGUSR1, signal_handler);

    printf("Child (Pong) process (PID: %d) started\n", getpid());

    while (1) 
    {
        pause();  /*Waits for SIGUSR1 from parent*/
        if (signal_received) 
        {
            printf("Child: Received SIGUSR1, sending SIGUSR2 to parent\n");
            kill(parent_pid, SIGUSR2);  /*Sends SIGUSR2 to parent*/
            signal_received = 0;
        }
    }

    return 0;
}
