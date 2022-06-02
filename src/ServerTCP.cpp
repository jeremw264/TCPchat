/**
 * @file ServerTCP.cpp
 * @author Jérémy Woirhaye (jerem.woirhaye@gmail.com)
 * @brief Server for TCP chat
 * @version 0.1
 * @date 2022-05-30
 */
#include "../include/ServerTCP.hpp"
#include "../include/Logger.hpp"

Logger logger;

/**
 * @brief Construct a new ServerTCP:: ServerTCP object
 * 
 * @param port Server port.
 */
ServerTCP::ServerTCP(unsigned short port)
{
    this->port = port;

    this->init();
}

ServerTCP::~ServerTCP()
{
}

/**
 * @brief Creation of the server socket.
 */
void ServerTCP::create_socket()
{

    // AF_INET pour utilisation du Protocoles Internet IPv4.
    // SOCK_STREAM support de dialoggerue garantissant l'intégrité.
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0)
    {
        logger.error("Problem during socket creation");
        exit(EXIT_FAILURE);
    }

    this->socket_fd = socket_fd;
}

/**
 * @brief Creation of the interface.
 */
void ServerTCP::create_interface()
{

    sockaddr_in addr;

    this->create_socket();

    // Accepte toute les connexions
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // Adresse IPV4
    addr.sin_family = AF_INET;
    // Le port choisi pour le serveur
    addr.sin_port = htons(this->port);

    if (bind(this->socket_fd, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        logger.error("Problem during socket binding");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Initialization of the server. 
 */
void ServerTCP::init()
{
    this->create_interface();

    if (listen(this->socket_fd, MAX_NB_CONNECTION) < 0)
    {
        logger.error("Problem during listening");
        exit(EXIT_FAILURE);
    }

    logger.info("Server TCP listen on port " + std::to_string(this->port) + "...");
}

/**
 * @brief Start the server.
 */
void ServerTCP::run()
{
    int i;

    while (1)
    {

        int client_socket;
        sockaddr_in client_addr;
        socklen_t clientAddrSize = sizeof(client_addr);
        struct client c;

        client_socket = accept(this->socket_fd, (sockaddr *)&client_addr, &clientAddrSize);

        if (client_socket < 0)
        {
            logger.error("Problem with client acceptance");
            exit(EXIT_FAILURE);
        }

        logger.info("A new client is accepted.");

        std::thread t_listener(&ServerTCP::listen_client, this, client_socket);

        this->clients.push_back(this->make_client(client_socket, move(t_listener)));
    }

    for (i = 0; i < (int)this->clients.size(); i++)
    {
        if (this->clients[i].t.joinable())
        {
            this->clients[i].t.join();
        }
    }

    this->clear_clients();
    this->closeServer();
}

/**
 * @brief Return a client built with the supplied parameter.
 * 
 * @param client_socket_fd The client socket.
 * @param t The client thread.
 * @return client The client builds.
 */
client ServerTCP::make_client(int client_socket_fd, std::thread t)
{
    struct client c;

    c.clientSocketFd = client_socket_fd;
    c.t = move(t);

    return c;
}

/**
 * @brief Listen to the client.
 * 
 * @param client_socket_fd  The client socket.
 */
void ServerTCP::listen_client(int client_socket_fd)
{

    char buffer[BUFFER_SIZE], name[BUFFER_SIZE];
    int connecting = 1;

    recv(client_socket_fd, name, sizeof(name) - 1, 0);

    // TODO gerer cas ou le client ne saisie pas de nom est ce deconncet direct

    logger.info("Client " + std::string(name) + " is connected.");

    while (connecting)
    {
        int r = recv(client_socket_fd, buffer, BUFFER_SIZE - 1, 0);
        char msg[BUFFER_SIZE];
        msg[0] = 0;

        if (r < 0)
        {
            logger.warning("Connection lost with the client.");
        }

        if (r == 0)
        {
            strncpy(msg, name, BUFFER_SIZE - 1);
            strncat(msg, " is disconnected.", sizeof(msg) - strlen(msg));
            this->broadcast_message(client_socket_fd, msg);
            logger.info("Client " + std::string(name) + " is disconnected.");
            this->remove_client(client_socket_fd);
            connecting = 0;
        }
        else
        {
            strncpy(msg, name, BUFFER_SIZE - 1);
            strncat(msg, " : ", sizeof(msg) - strlen(msg) - 1);
            strncat(msg, buffer, sizeof(msg) - strlen(msg) - 1);
            this->broadcast_message(client_socket_fd, msg);
        }
    }
}

/**
 * @brief Sends a message to all clients.
 *
 * @param client_socket_fd The socket_fd of sender of the message.
 * @param buffer The buffer that contains the message.
 */
void ServerTCP::broadcast_message(int client_socket_fd, const char *buffer)
{
    int i;

    for (i = 0; i < (int)this->clients.size(); i++)
    {
        if (this->clients[i].clientSocketFd != client_socket_fd)
        {
            if (send(this->clients[i].clientSocketFd, buffer, BUFFER_SIZE, 0) < 0)
            {
                logger.error("Problem with send for broadcast.");
                exit(EXIT_FAILURE);
            }
        }
    }
}

/**
 * @brief Removes a client from the server.
 *
 * @param client_socket_fd The client socket_fd.
 */
void ServerTCP::remove_client(int client_socket_fd)
{
    int i;

    for (i = 0; i < (int)this->clients.size(); i++)
    {
        if (this->clients[i].clientSocketFd == client_socket_fd)
        {
            this->clients[i].t.detach();
            close(client_socket_fd);
            this->clients.erase(this->clients.begin() + i);
            return;
        }
    }
}

/**
 * @brief Removes all clients from the server.
 */
void ServerTCP::clear_clients()
{
    int i;
    for (i = 0; i < (int)this->clients.size(); i++)
    {
        this->remove_client(this->clients[i].clientSocketFd);
    }
}

/**
 * @brief Closes the server.
 */
void ServerTCP::closeServer()
{
    close(this->socket_fd);
}
