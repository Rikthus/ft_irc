#include "../includes/Server.hpp"

bool portVerif(char *str)
{
    int value;

    for (int i = 0; str[i], i++)
    {
        if (i > 3 || !std::isdigit(str[i]))
            return (false);
    }
    value = std::atoi(str);
    if (value < 6660 && value > 6669 && value != 7000)
        return (false);
    return (true);
}

int main(int argc, char **argv)
{
    if (argc != 3 || !argv[1] || !argv[2] || argv[2][0] == '\0')
    {
        std::cerr << "Error: bad usage" << std::endl;
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return (0);
    }
    else if (!portVerif(argv[1]))
    {
        std::cerr << "Error: invalid port (6660-6669 or 7000)" << std::endl;
        return (0);
    }

    Server  ircServer(argv[1], argv[2]);
    
}