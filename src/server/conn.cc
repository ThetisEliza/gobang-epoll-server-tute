#include <string>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

#include "conn.hh"
#include "../utils/utils.hh"

Socket::Socket():fd(-1)
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    Errif(fd == -1, "socket create error");
}

Socket::Socket(int _fd): fd(_fd) 
{
    Errif(fd == -1, "socket create error");
}

Socket::~Socket() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

void Socket::Bind(const INET_ADDR *addr)
{
    int flag = 1;
    Errif(::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1, "set socket opt error");
    Errif(::bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1, "socket bind error");
}

void Socket::Listen()
{
    Errif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}


void Socket::Connect(INET_ADDR *addr)
{
    Errif(::connect(fd, (sockaddr*)&(addr->addr), sizeof(addr->addr)), "connecting to server failed");
}

void Socket::SetNonBlocking()
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::Accept(InetAddress *addr)
{
    int clntSocketFd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    Errif(clntSocketFd == -1, "socket accept error");
    return clntSocketFd;
}

int Socket::GetFd()
{
    return fd;
}



#define MAX_EVENT_NUM 10000


void AddFd(int epollFd, int fd, bool oneshot)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    if (oneshot)
        event.events |= EPOLLONESHOT;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void Server::Run()
{
    Socket serverSocket;
    
    std::cout << "Server Binding addr" << std::endl;
    serverSocket.Bind(&SERVER_ADDR);
    std::cout << "Server Listening at:" << SERVER_ADDR.ToString() << std::endl;
    serverSocket.Listen();
    std::cout << "Server Set Non-blocking" << std::endl;
    serverSocket.SetNonBlocking();

    // epoll_fd repr. the epoll table in kernel
    int epollFd = epoll_create(5);
    

    // buffer for ready events 
    epoll_event events[MAX_EVENT_NUM];


    // MASTER thread register socket fd, listen fd get ready when new connection comes
    AddFd(epollFd, serverSocket.GetFd(), false);
    
    while (1) {

        int num = epoll_wait(epollFd, events, MAX_EVENT_NUM, -1);

        for(int i=0; i<num; i++) {
            int socketFd = events[i].data.fd;
            std::cout << "Check socket fd:" << socketFd << std::endl;  
            
            if (socketFd == serverSocket.GetFd()) {
                std::cout << "New connection event at server socket" << events[i].data.fd << std::endl;    
                /* process new connection */

                INET_ADDR connectionAddr; // I feel this addr useless, no change after socket accept, why?
                // std::cout << "Prepare new socket at:" << connectionAddr.ToString() << std::endl;

                Socket clientSocket = serverSocket.Accept(&connectionAddr); // A socket is just a FILE DESCRIBER in linux kernel.
                /* Accept connection on new socket*/
                std::cout << "New connection on fd:" << clientSocket.GetFd() << std::endl;
                AddFd(epollFd, clientSocket.GetFd(), true);            
                std::cout << "Connection created on fd:" << clientSocket.GetFd() << std::endl;
            }
            else if ( events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR) ) {
                std::cout << "Server closed connect" << std::endl;
                /* what does this mean? Server actively close connection? */
            }
            else if ( events[i].events & EPOLLIN ) {
                std::cout << "Server received data" << std::endl;
            }
            else if ( events[i].events & EPOLLOUT ){
                std::cout << "Server sent data" << std::endl;
            }            
        }
    }

    close(epollFd);
}