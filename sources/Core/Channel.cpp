#include "../../includes/Core/Channel.hpp"

//////**********************************//////
//////				PUBLIC				//////
//////**********************************//////

//////////////////////////
//		FUNCTIONS		//
//////////////////////////

bool	Channel::findInvite(int clientSockfd) const
{
	if (!mInviteOnly)
		return (true);
	for (unsigned long i = 0; i < mInvited.size(); i++)
	{
		if (mInvited[i] == clientSockfd)
			return (true);
	}
	return (false);
}

bool	Channel::checkPwd(std::string key) const
{
	if (mPassProtected && key != mPwd)
		return (false);
	else if (!mPassProtected && key != NO_PWD)
		return (false);
	else
		return (true);
}

bool	Channel::checkSpace(void) const
{
	if (mCapped && mClientList.size() == static_cast<unsigned long>(mMaxCapacity))
		return (false);
	else
		return (true);
}

void	Channel::addClient(int clientSockfd, Client &clientData)
{
	if (mClientList.find(clientSockfd) != mClientList.end())
	{
		std::cout << "CLIENT ALREADY IN CHANNEL" << std::endl;
		return ;
	}
	mClientList.insert(std::pair<int, Client &>(clientSockfd, clientData));
	if (mCapped)
		mMaxCapacity++;
	for (std::vector<int>::iterator it; it != mInvited.end(); it++)
	{
		if (*it == clientSockfd)
			mInvited.erase(it);
	}
	std::cout << "ADDED TO CHAN" << std::endl;
}

//////////////////////////
//		BUILDERS		//
//////////////////////////

Channel::Channel(std::string chanName, int creatorSockfd, Client &creatorData, bool isPwd, std::string pwd)
				: mName(chanName), mAdmin(creatorSockfd)
{
	if (isPwd)
	{
		mPassProtected = true;
		mPwd = pwd;
	}
	else
	{
		mPassProtected = false;
		mPwd = NO_PWD;
	}
	mClientList.insert(std::pair<int, Client &>(mAdmin, creatorData));
	mOperators.push_back(creatorSockfd);
	mInviteOnly = false;
	mCapped = false;
	mMaxCapacity = 0;
}

Channel::~Channel(void)	{}

//////**********************************//////
//////				PRIVATE				//////
//////**********************************//////

