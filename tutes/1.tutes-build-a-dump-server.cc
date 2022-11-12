#include <iostream>
#include <unistd.h>
#include <string>

#include "../src/server/conn.hh"


std::string s(R"de(HTTP/1.1 200 OK
Server: Apache-Coyote/1.1
Content-Type: text/html;charset=UTF-8
Connection: keep-alive


<!DOCTYPE HTML">
<html>
  <head>
    Hello world;
  </head>
  <body>
    This is my JSP page. <br>
  </body>
<html>

)de");

int main()
{
    Socket serverSocket;

    serverSocket.Bind(&SERVER_ADDR);  
    serverSocket.Listen();

    INET_ADDR clntAddr;
    int connFd = serverSocket.Accept(&clntAddr);        
    std::cout << "Remote connection at:" << clntAddr.ToString() << std::endl;

    char buf[1024];
    read(connFd, (void*)buf, 1024);
    std::cout << "Revice from connection:" << "\n" << buf << std::endl; 
    
    const char* sendData = s.c_str();
    size_t len = s.size();
    ssize_t sent = write(connFd, (const void*)sendData, len);
    std::cout << "Sent to connection:" << sent << std::endl; 
    std::cout << "Sent to connection:" << sendData << std::endl; 
}