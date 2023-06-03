#include "../../includes/Core/Channel.hpp"

//////**********************************//////
//////				PUBLIC				//////
//////**********************************//////

//////////////////////////
//		FUNCTIONS		//
//////////////////////////

bool	Channel::findInvite(int clientSockfd)
{
	if (!mInviteOnly)
		return (true);
	for (std::vector<int>::iterator it = mInvited.begin(); it != mInvited.end(); it++)
	{
		if (*it == clientSockfd)
		{
			mInvited.erase(it);
			return (true);
		}
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
		return ;
	mClientList.insert(std::pair<int, Client *>(clientSockfd, &clientData));
	if (mCapped)
		mMaxCapacity++;
	for (std::vector<int>::iterator it = mInvited.begin(); it != mInvited.end(); it++)
	{
		if (*it == clientSockfd)
			mInvited.erase(it);
	}
}

void	Channel::addInvitation(int isInvited)
{
	mInvited.push_back(isInvited);
}

bool	Channel::sockClientIsInChan(int clientSockfd)
{
	std::map<int, Client *>::iterator	it;

	it = mClientList.find(clientSockfd);
	if (it != mClientList.end())
		return (true);
	else
		return (false);
}

bool	Channel::clientIsInChan(std::string clientName) const
{
	std::map<int, Client *>::const_iterator	it;

	for (it = mClientList.begin(); it != mClientList.end(); it++)
	{
		if (it->second->getNickname() == clientName)
			return (true);
	}
	return (false);
}

bool	Channel::sockClientIsOperator(int clientSockfd)
{
	for (unsigned long i = 0; i < mOperators.size(); i++)
	{
		if (mOperators[i] == clientSockfd)
			return (true);
	}
	return (false);
}

void	Channel::rplTopic(void)
{
	std::map<int, Client *>::iterator	it;

	for (it = mClientList.begin(); it != mClientList.end(); it++)
		Rep().R332(it->first, it->second->getNickname(), mName, mTopic);
}

//////////////////////////
//		BUILDERS		//
//////////////////////////

Channel::Channel(std::string chanName, int creatorSockfd, Client &creatorData, bool isPwd, std::string pwd)
				: mName(chanName)
{
	if (isPwd)
	{
		mPassProtected = true;
		mPwd = pwd;
	}
	else
	{
		mPassProtected = false;
		mPwd = "\0";
	}
	mClientList.insert(std::pair<int, Client*>(creatorSockfd, &creatorData));
	mOperators.push_back(creatorSockfd);
	mTopic = "\0";
	mTopicProtected = false;
	mInviteOnly = false;
	mCapped = false;
	mTopicSet = false;
	mMaxCapacity = 0;
}

Channel::~Channel(void)	{}

//////////////////////
//		GETTERS		//
//////////////////////

bool	Channel::getTopicProtected(void) const {return (mTopicProtected);}
bool	Channel::getPassProtected(void) const {return (mPassProtected);}
bool	Channel::getInviteOnly(void) const {return (mInviteOnly);}
bool	Channel::getCapped(void) const {return (mCapped);}
bool	Channel::getTopicSet(void) const {return (mTopicSet);}
std::string	Channel::getTopic(void) const {return (mTopic);}

std::string	Channel::getMods(void)
{
	std::string	ret;

	ret.append("+");
	if (mTopicProtected)
		ret.append("t");
	if (mPassProtected)
		ret.append("k");
	if (mInviteOnly)
		ret.append("i");
	if (mCapped)
		ret.append("l");
	if (ret.size() == 1)
		ret = "\0";
	return (ret);
}

//////////////////////
//		SETTERS		//
//////////////////////

void	Channel::setTopicProtected(bool mode) {mTopicProtected = mode;}

void	Channel::setPass(bool mode, std::string newPass)
{
	if (mode == true)
	{
		mPwd = newPass;
		mPassProtected = true;
	}
	else
	{
		mPwd = "\0";
		mPassProtected = false;
	}
}

void	Channel::setInviteOnly(bool mode)
{
	if (mode == true)
		mInviteOnly = true;
	else
	{
		mInviteOnly = false;
		mInvited.clear();
	}
}

void	Channel::setCapped(bool mode, unsigned int newSize)
{
	if (mode == true)
	{
		if (newSize > MAX_CHAN_CAPACITY)
			mMaxCapacity = MAX_CHAN_CAPACITY;
		else
			mMaxCapacity = newSize;
		mCapped = true;
	}
	else
	{
		mMaxCapacity = 0;
		mCapped = false;
	}
}

void	Channel::setTopic(bool mode, std::string newTopic)
{
	if (mode == true)
	{
		mTopic = newTopic;
		mTopicSet = true;
	}
	else
	{
		mTopic = "\0";
		mTopicSet = false;
	}
}

int	Channel::setOperator(bool mode, std::string nick)
{
	std::map<int, Client *>::iterator	it;
	int									userSockfd;

	for (it = mClientList.begin(); it != mClientList.end(); it++)
	{
		if (it->second->getNickname() == nick)
			break ;
	};
	if (it == mClientList.end())
		return (-1);
	userSockfd = it->second->getFd();
	for (std::vector<int>::iterator itVec = mOperators.begin(); itVec != mOperators.end(); itVec++)
	{
		if (*itVec == userSockfd)
		{
			if (mode == false)
				mOperators.erase(itVec);
			return (0);
		}
	}
	if (mode == true)
		mOperators.push_back(userSockfd);
	return (0);
}

//////**********************************//////
//////				PRIVATE				//////
//////**********************************//////

