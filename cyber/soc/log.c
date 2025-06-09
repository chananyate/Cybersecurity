/* This program generates various logging messages with different severity levels, 
   and these messages are stored in the system log files like /var/log/syslog */

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main() {
    /* Opens connection to syslog */
    openlog("LoggingProgram", LOG_PID | LOG_CONS, LOG_USER);

    /* Logs various messages with different severities */
    syslog(LOG_INFO, "This is an informational message");
    syslog(LOG_WARNING, "This is a warning message");
    syslog(LOG_ERR, "This is an error message");
    syslog(LOG_CRIT, "This is a critical message");

    /* Closes connection to syslog */
    closelog();

    return 0;
}
