#ifndef JOIN_HPP
# define JOIN_HPP

# include "../Core/ft_irc.hpp"

class ACmd
{
	public:
		virtual	void	execute(Server *server, int clientSockfd, Client &clientData, std::string msg) = 0;
		ACmd(void);
		virtual	~ACmd(void);
};

class JOIN: public ACmd
{
	public:
		void execute(Server *server, int clientSockfd, Client &clientData, std::string msg);

		JOIN(void);
		~JOIN(void);
};

#endif
