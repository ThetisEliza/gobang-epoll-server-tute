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

int pipes[2];

#define BUFFER_SIZE 1024

/**
 * @brief This is a tutorial that showing you how to use `sendfile` to send data to a designated file descriptor
 * 
 * @return int 
 */
int main()
{
    char file_name[] = "tmp/a.txt";

    int file_desc = open(file_name, O_RDONLY);
    
    struct stat stat_buf;
    fstat(file_desc, &stat_buf);
    sendfile(STDOUT_FILENO, file_desc, NULL, stat_buf.st_size);

    close(file_desc);
    
    return 0;
}