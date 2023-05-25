#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <unistd.h>

class	Client
{
	public:

		Client(int	socketFd, std::string nickname);
		~Client(void);


	private:

		int				mClientSocketFd;
		std::string		mNickname;

		Client(Client const &rSrc);
		Client(void);
		Client	&operator=(Client const &rRhs);
};

#endif