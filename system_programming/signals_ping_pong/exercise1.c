#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*Shared variable to indicate that a signal was received*/
volatile sig_atomic_t signal_received = 0;

/*Signal handler function: sets the signal_received flag to 1 when a signal is caught.
  signo is the signal number that triggered this handler. It allows the handler 
  to identify which signal was received, enabling the handling of multiple signals 
  with the same function if needed.*/
void signal_handler(int signo)   
{
    if (signo == SIGUSR1) 
        printf("Received SIGUSR1\n");
    else if (signo == SIGUSR2) 
        printf("Received SIGUSR2\n");
    
    signal_received = 1;
}


int main() 
{
    pid_t pid;
    /*Sets the number of signal exchanges between parent and child*/
    int count = 5;
    
    signal(SIGUSR1, signal_handler);  /*Calls signal_handler whenever the process receives the SIGUSR1 signal.*/
    signal(SIGUSR2, signal_handler);
    
    /*Creates a new process (child process)*/
    /*printf("chananya\n");*/
    pid = fork();

    /*printf("Uri\n");*/

    
    if (pid < 0) 
    {
        perror("Fork failed");
        return 1;
    }
    
    if (pid == 0) 
    {  /*Child process*/
        while (count--)  /*Runs for the specified number of exchanges*/
        {
            /*printf("I am in child before pause\n");*/

            pause();  /*Suspends the process until it receives a signal, avoiding unnecessary CPU usage while waiting for the signal.*/
            if (signal_received) 
            {
                printf("Child: Received SIGUSR1, sending SIGUSR2 to parent\n");
                kill(getppid(), SIGUSR2);  /*Sends SIGUSR2 to the parent process*/
                signal_received = 0;
            }
        }
    } 
    else 
    {  /*Parent process*/
        while (count--)  
        {
            sleep(1);  /*Waits 1 second before each signal to avoid overwhelming the child*/
            printf("Parent: Sending SIGUSR1 to child\n");
            kill(pid, SIGUSR1);  /*Sends SIGUSR1 to the child*/
                        /*printf("I am in parent before pause\n");*/

            pause();  /*Waits for SIGUSR2*/
            if (signal_received) 
            {
                printf("Parent: Received SIGUSR2\n");
                signal_received = 0;
            }
        }
    }
    
    return 0;
}