#include <iostream>

#include "server/conn.hh"

int main()
{
    std::cout << "Hello" << std::endl;
    // std::cout << SERVER_ADDR.ToString() << std::endl;
    // std::cout << SERVER_ADDR.addr_len << std::endl;
    Server server;
    server.Run();

    
}