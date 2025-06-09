/*ping*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

volatile sig_atomic_t signal_received = 0;

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
    
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);
    
    pid = fork();
    
    if (pid < 0) 
    {
        perror("Fork failed");
        return 1;
    }
    
    else if (pid == 0) 
    {  /*Child process*/
        /*Execute new program*/
        char pid_str[20];
        /*Gets the PID of the parent process using getppid() and formats it as a string using sprintf(), 
          storing the result in pid_str.*/
        sprintf(pid_str, "%d", getppid());  

        /*The first argument is the path to the executable to run, the second is the name of the program 
          as it will appear in the process listing (usually the same as the executable name), 
          and the third is the argument to pass to the new program (which is the parent's PID in string format). 
          The last argument (NULL) signifies the end of the argument list.*/
        execl("./pong", "pong", pid_str, NULL);
        perror("Exec failed");
        return 1;
    }
    
    else
    {
        printf("Parent process (PID: %d) started\n", getpid());
        printf("Child process (PID: %d) started\n", pid);
    
        while (1) 
        {
            sleep(1);
            kill(pid, SIGUSR1);
            printf("Parent: Sent SIGUSR1\n");
        
            pause();
            if (signal_received) 
            {
                printf("Parent: Received SIGUSR2\n");
                signal_received = 0;
            }
        }
    }
    
    return 0;
}