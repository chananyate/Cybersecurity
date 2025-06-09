/*Pong.c*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
/*#include <fcntl.h>*/

volatile sig_atomic_t signal_received = 0;

void signal_handler(int signum) 
{
    signal_received = 1;
    printf("Process 2 received signal from Process 1\n");
}

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <process1_pid>\n", argv[0]);
        return 1;
    }

    /*Gets Process 1's PID from command line argument*/
    pid_t other_pid = atoi(argv[1]);
    if (other_pid <= 0) 
    {
        fprintf(stderr, "Invalid PID provided\n");
        return 1;
    }

    /*Stores Pong's PID in a file for Ping to read*/
    pid_t my_pid = getpid();
    FILE *pid_file = fopen("process2.pid", "w");
    if (pid_file == NULL) 
    {
        perror("Failed to create PID file");
        return 1;
    }
    fprintf(pid_file, "%d", my_pid);  /*Writes PID to file*/
    fclose(pid_file);

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    
    if (sigaction(SIGUSR1, &sa, NULL) == -1) 
    {
        perror("sigaction");
        return 1;
    }

    printf("Process 2 (PID: %d) starting...\n", my_pid);
    printf("Sending signals to Process 1 (PID: %d)\n", other_pid);

    /*Start the signal exchange by sending first signal*/
    printf("Process 2 sending first signal to Process 1\n");
    if (kill(other_pid, SIGUSR1) == -1) {
        perror("Failed to send signal to Process 1");
        return 1;
    }

    while (1) {
        if (signal_received) 
        {
            sleep(2);  
            kill(other_pid, SIGUSR1);
            printf("Process 2 sent signal to Process 1\n");
            signal_received = 0;
        }
        sleep(1);
    }

    return 0;
}