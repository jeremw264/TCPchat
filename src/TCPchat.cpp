#include "../include/ServerTCP.hpp"
#include "../include/ClientTCP.hpp"
#include "../include/Logger.hpp"


#include <iostream>

Logger l;

int main(int argc, char const *argv[])
{

    unsigned short port = 7654;

    if (argc < 2)
    {
        std::cout << "./TCPchat.cpp <start_mode> (S or C)" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (*argv[1] == 's' || *argv[1] ==  'S')
    {
        l.info("TCPchat start in Server mode");
        ServerTCP serverTCP = ServerTCP(port);
        serverTCP.run();
    }else if (*argv[1] == 'c' || *argv[1] ==  'C')
    {
        l.info("TCPchat start in Client mode");
        ClientTCP clientTCP = ClientTCP(argv[2],port);
        clientTCP.run();
    }else
    {
        std::cout << "Mode not found." << std::endl;
    }
    
    
    return 0;
}

