#ifndef SERVER_TCP_HPP
#define SERVER_TCP_HPP

#define MAX_NB_CONNECTION 8
#define BUFFER_SIZE 1024

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdlib.h>
#include <vector>
#include <cstring>

#include <thread>

struct client
{
    int clientSocketFd;
    char name[BUFFER_SIZE];
    std::thread t;
} ;


class ServerTCP
{
private:
    unsigned short port;
    int socket_fd;
    std::vector<client> clients;

    void create_socket();
    void create_interface();
    void init();
    void listen_client(int client_socket_fd);
    void broadcast_message(int client_socket_fd, const char *buffer);
    void remove_client(int client_socket_fd);
    void clear_clients();
    void closeServer();

    client make_client(int client_socket_fd,std::thread t);

public:
    ServerTCP(unsigned short port);
    ~ServerTCP();
    void run();
};

#endif