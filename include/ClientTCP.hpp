#ifndef CLIENT_TCP_HPP
#define CLIENT_TCP_HPP

#define MAX_BUFFER 1024

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <thread>

#include <iostream>
#include <string.h>

class ClientTCP
{
private:
    int socket_fd;
    unsigned short server_port;
    int server_connection;
    std::string server_addr;
    std::thread t_emit, t_receive;
    
    void start();
    void send_to_server();
    void receive_by_server();

public:
    ClientTCP(std::string server_addr,unsigned short server_port);
    ~ClientTCP();
    void run();
};

#endif