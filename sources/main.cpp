#include "../includes/Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3 || !argv[1] || !argv[2] || argv[2][0] == '\0')
    {
        std::cerr << "Error: bad usage" << std::endl;
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return (0);
    }
    try
    {
        Server  ircServer(argv[1], argv[2]);
        ircServer.launch();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}
