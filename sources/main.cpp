#include "../includes/Server.hpp"

void    parseArgs

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Error: bad usage" << std::endl;
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return (0);
    }
    
}