#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <memory.h>
#include <sys/uio.h>
#include <sys/sendfile.h>
#include <syslog.h>


/**
 * @brief This is a tutorial that showing you how to use `sendfile` to send data to a designated file descriptor
 * 
 * @return int 
 */
int main()
{

    setlogmask( LOG_UPTO(LOG_NOTICE) );
    
    openlog(NULL, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

    syslog(LOG_NOTICE, "Hello world");
    
    closelog();

    return 0;
}