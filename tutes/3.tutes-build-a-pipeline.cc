#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/wait.h>

#include "../src/server/conn.hh"

int pipe_default[2];

int main()
{
    pid_t pid;
    char buffer[32];
    if (pipe(pipe_default) < 0) {
        std::cout << "Failed to create pipe!\n" << std::endl;
    }
    std::cout << "r_pid:" << pipe_default[0] 
        << " w_rid:" << pipe_default[1] << std::endl;

    if(0 == (pid = fork()))
    {
        printf("This is child process\n");
        std::cout << "child pipe close write pipe" << std::endl;
        close(pipe_default[1]);
        sleep(5);
        if(read(pipe_default[0], buffer, 32) > 0)
        {
            printf("Receive data from server, %s!\n", buffer);
        }
        close(pipe_default[0]);
    }
    else
    {
        printf("This is parent process\n");
        std::cout << "child pipe close read pipe" << std::endl;
        close(pipe_default[0]);
        if(-1 != write(pipe_default[1], "hello", strlen("hello")))
        {
            printf("Send data to client, hello!\n");
        }
        close(pipe_default[1]);
        
        waitpid(pid, NULL, 0);
    }
}