/*
 * @Date: 2022-11-14 19:37:53
 * @LastEditors: ThetisEliza wxf199601@gmail.com
 * @LastEditTime: 2022-11-15 20:41:20
 * @FilePath: /gobang-epoll-server-tute.hub/src/server/conn.hh
 */
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <functional>
#include <memory.h>
#include <string>
#include <vector>

#ifndef INETADDR
#define INETADDR

#define MAX_EVENT_NUM   10000

#define PORT            8899
#define IP              "127.0.0.1"


/**
 * @brief The Defination of an IP4 address capsule.
 */
struct InetAddress 
{
    struct sockaddr_in addr; // addr for linux
    socklen_t addr_len;  // addr size
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
        in_port_t port = ntohs(addr.sin_port);
        return std::string("ip:").append(ip).append(" port:").append(std::to_string(port));
    }
};

typedef InetAddress INET_ADDR;

extern const INET_ADDR SERVER_ADDR;

#endif


/**
 * @brief Socket Capsule
 * 
 */
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

public:
    static INET_ADDR GetLocalHost();
};



class Server
{
public:
    Server() {};
    ~Server() {};
    void Run();
};