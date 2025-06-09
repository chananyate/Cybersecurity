/*Ping.c*/
#include <stdio.h> 
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
/*#include <fcntl.h>*/

/*struct sigaction: A structure used to specify the action to be associated with a specific signal.
  It's defined in <signal.h> and contains fields for the signal handler function (sa_handler or sa_sigaction),
  a signal mask (sa_mask) to block certain signals during the execution of the handler,
  and flags (sa_flags) to modify the behavior of the signal handling.*/


volatile sig_atomic_t signal_received = 0;

void signal_handler(int signum) 
{
    signal_received = 1;
    printf("Process 1 received signal from Process 2\n");
}

int main() 
{
    pid_t my_pid = getpid();
    
    /*Set up the signal handler for SIGUSR1*/
    struct sigaction sa;  
    sa.sa_handler = signal_handler;  /*Assigns the signal handler*/ 
    sa.sa_flags = 0;  /*No special flags*/
    sigemptyset(&sa.sa_mask);  /*Initializes the signal mask to empty*/
    
    /*Registers the signal handler for SIGUSR1 using sigaction.
      This function replaces the default action for SIGUSR1 with the handler specified in the sa structure.
      If sigaction fails (like if the signal is invalid), it returns -1 and the error is printed using perror.*/
    if (sigaction(SIGUSR1, &sa, NULL) == -1) 
    {
        perror("sigaction");
        return 1;
    }

    printf("Process 1 (PID: %d) waiting for signals...\n", my_pid);

    while (1) 
    {
        if (signal_received) 
        {
            /*Opens the file containing Process 2's PID*/
            FILE *other_pid_file = fopen("process2.pid", "r");
            if (other_pid_file != NULL) 
            {
                pid_t other_pid;
                fscanf(other_pid_file, "%d", &other_pid);
                fclose(other_pid_file);
                /*Sends SIGUSR1 signal back to Process 2*/
                kill(other_pid, SIGUSR1);
                printf("Process 1 sent signal to Process 2\n");
            }
            signal_received = 0;  /*Resets the signal received flag*/
        }
        sleep(1);
    }

    return 0;
}
