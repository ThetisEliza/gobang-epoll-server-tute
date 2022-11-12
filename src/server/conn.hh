#include <arpa/inet.h>
#include <sys/epoll.h>
#include <functional>
#include <memory.h>
#include <string>
#include <vector>

#ifndef INETADDR
#define INETADDR
struct InetAddress 
{
    struct sockaddr_in addr;
    socklen_t addr_len;
    InetAddress() {
        memset((void*)&addr, 0, sizeof(addr));
        addr.sin_family         = AF_INET;   
        addr_len                = sizeof(addr); 
    };
    InetAddress(const std::string ip, uint16_t port) 
    {
        memset((void*)&addr, 0, sizeof(addr));
        addr.sin_family         = AF_INET;
        addr.sin_addr.s_addr    = inet_addr(ip.c_str());
        addr.sin_port           = htons(port);
        addr_len                = sizeof(addr);
    }
public:
    std::string ToString() const
    {
        char* ip = inet_ntoa(*(struct in_addr*)(&(addr.sin_addr.s_addr)));
        int   port = addr.sin_port;
        return std::string("ip:").append(ip).append(" port:").append(std::to_string(port));
    }
};

typedef InetAddress INET_ADDR;

const INET_ADDR SERVER_ADDR = INET_ADDR("10.0.4.2", 8899);

#endif



class Socket 
{
private:
    int fd;
public:
    Socket();
    Socket(int);
    ~Socket();
    void    Bind(const INET_ADDR*);
    void    Listen();
    void    SetNonBlocking();
    int     Accept(INET_ADDR*);
    int     GetFd();
    void    Connect(INET_ADDR*);
};



class Server
{
public:
    Server() {};
    ~Server() {};
    void Run();
};