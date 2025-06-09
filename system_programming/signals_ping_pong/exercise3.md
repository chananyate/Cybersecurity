1. When a process is sleeping and receives a signal, what happens?
When a process is sleeping (in my case, waiting in the pause() function) and it receives a signal, the following occurs:
The process is interrupted from its sleep state, and control is transferred to the signal handler for that signal.
In the GDB output, we can see this process flow: 
the program was in __libc_pause() (indicating it was sleeping), and then it received SIGUSR1. The signal handler was invoked, as evidenced by the backtrace showing that control moved from pause() to the signal_handler.

After the signal is handled, the process can either resume where it left off (if it was not terminated) or continue executing its normal flow, typically going back to a sleeping state if it reaches another pause().

2. What does the stack look like when handling a signal?
When a signal is handled, the stack will look something like this:
The stack frame for the signal handler function (like signal_handler) is pushed onto the stack. This frame includes the signo parameter, which contains the signal number (in this case, 10 for SIGUSR1).
The GDB output shows the backtrace when the signal handler is invoked:
  #0  signal_handler (signo=10) at exercise2b.c:11
  #1  <signal handler called>
  #2  0x0000ffff95856b50 in __libc_pause () at ../sysdeps/unix/sysv/linux/pause.c:31
  #3  0x0000aaaaaeec0a78 in main (argc=2, argv=0xffffd3b944d8) at exercise2b.c:33

This indicates:
The first frame (#0) is the signal handler, showing the parameter signo.
The second frame (#1) is marked as <signal handler called>, indicating the transition from the main flow of the program to the signal handler.
The third frame (#2) shows the pause() function, which was interrupted by the signal.
#3 indicates that the signal handler was invoked while in the main function, specifically at line 33, where pause() was called.

3. What happens when a signal is being sent to a process that is busy?
When a signal is sent to a process that is currently busy (executing code and not waiting in a state like pause()), the following occurs:
The signal is queued for that process. Depending on the type of signal and the signal handling behavior defined (default action, ignoring, or custom handler), the process may or may not be interrupted.
If the signal is a real-time signal (like SIGUSR1 or SIGUSR2), it can interrupt the current operation and invoke the specified signal handler immediately.
In my case, if the parent process (ping) is busy when the child (pong) sends SIGUSR2, the signal would be queued until the process is ready to handle it (when the parent returns to a state where it can process signals).
If the parent were actively executing code and not set up to handle that signal, the program could continue executing until it reaches a point where it can respond to the signal, such as returning to pause().


