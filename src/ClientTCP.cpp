/**
 * @file ClientTCP.cpp
 * @author Jérémy Woirhaye (jerem.woirhaye@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-06-01
 */

#include "../include/ClientTCP.hpp"
#include "../include/Logger.hpp"

Logger loggerr;

ClientTCP::ClientTCP(std::string server_addr, unsigned short server_port)
{
    this->server_addr = server_addr;
    this->server_port = server_port;
}

ClientTCP::~ClientTCP()
{
}

void ClientTCP::start()
{

    int socket_fd;
    sockaddr_in client_addr;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0)
    {
        loggerr.error("Problem during socket creation.");
        exit(EXIT_FAILURE);
    }

    this->socket_fd = socket_fd;

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(this->server_port);
    client_addr.sin_addr.s_addr = inet_addr(this->server_addr.c_str());

    if (connect(this->socket_fd, (sockaddr *)&client_addr, sizeof(sockaddr_in)) < 0)
    {
        loggerr.error("Problem during socket connection.");
        exit(EXIT_FAILURE);
    }

    this->server_connection = 1;
}

void ClientTCP::run()
{

    char name[MAX_BUFFER];

    this->start();

    std::cout << "What's your name ?" << std::endl;
    std::cin.getline(name, MAX_BUFFER - 1);
    send(this->socket_fd, name, sizeof(name), 0);

    std::thread emitter(&ClientTCP::send_to_server, this);
    std::thread receiver(&ClientTCP::receive_by_server, this);

    this->t_emit = move(emitter);
    this->t_receive = move(receiver);

    if (this->t_emit.joinable())
        this->t_emit.join();
    if (this->t_receive.joinable())
        this->t_receive.join();
}

void ClientTCP::send_to_server()
{

    char msg[MAX_BUFFER - 1];

    while (1)
    {
        if (this->server_connection)
        {
            std::cin.getline(msg, sizeof(msg));
        }else{
            break;
        }
        

        if (!strcmp((char *)"/exit", msg))
        {
            loggerr.info("Exit");
            this->t_receive.detach();
            break;
        }

        if (send(this->socket_fd, msg, sizeof(msg), 0) < 0)
        {
            loggerr.error("Problem with send");
            exit(EXIT_FAILURE);
        }
    }
}

void ClientTCP::receive_by_server()
{

    char msg[MAX_BUFFER];
    int error_code;
    socklen_t error_code_size = sizeof(error_code);

    while (1)
    {
        
        int r_bytes = recv(this->socket_fd, msg, sizeof(msg), 0);

        
        if (r_bytes < 0)
        {
            loggerr.error("Problem with recv.");
            exit(EXIT_FAILURE);
        }

        getsockopt(this->socket_fd, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);

        if (error_code <= 0)

        {
            loggerr.warning("Connection lost with the server");
            this->t_emit.detach();
            this->server_connection = 0;
            break;
        }

        std::cout << msg << std::endl;
    }
}
