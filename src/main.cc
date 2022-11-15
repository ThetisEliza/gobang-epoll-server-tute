/*
 * @Date: 2022-11-14 19:37:53
 * @LastEditors: ThetisEliza wxf199601@gmail.com
 * @LastEditTime: 2022-11-15 20:39:31
 * @FilePath: /gobang-epoll-server-tute.hub/src/main.cc
 */
#include <iostream>

#include "server/conn.hh"

int main()
{
    std::cout << "Hello" << std::endl;
    Server server;
    server.Run();
    // INET_ADDR ia = SERVER_ADDR;    
    // std::cout << ia.ToString() << std::endl;
}