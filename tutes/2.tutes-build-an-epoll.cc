#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <fcntl.h>
#include <sys/epoll.h>

#include "../src/server/conn.hh"


int main()
{
    int epfd, nfds;
    struct epoll_event ev1, ev2, events[5];

    epfd = epoll_create(5);
    ev1.data.fd = STDIN_FILENO;
    ev1.events = EPOLLIN | EPOLLET;

    /*I don't know how to monitor a TRUE `FILE` decsciptor. Maybe update this later*/
    ev2.data.fd = open("tmp/a.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IROTH);
    ev2.events = EPOLLIN | EPOLLET | EPOLLOUT |EPOLLERR;

    std::cout << ev2.data.fd << std::endl;

    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev1);
    epoll_ctl(epfd, EPOLL_CTL_ADD, ev2.data.fd, &ev2);
    
    char a[1024];

    for(;;) {
        nfds = epoll_wait(epfd, events, 5, -1);
        std::cout << "Coming number of events:" << nfds << std::endl;
        for(int i=0;i<nfds;i++) {
            if (events[i].data.fd == STDIN_FILENO) {
                printf("Hello epoll's world\n");
                std::cout << ev2.data.fd << std::endl;
                char wr_buf[100] = "hello world";
                int wr_ret = write(ev2.data.fd, wr_buf, sizeof(wr_buf));
                int res = read(ev2.data.fd, (void*)a, 1024);
                std::cout << res << std::endl;
            } else {
                printf("Hello epoll out's world\n");
            }
        }
    }


}