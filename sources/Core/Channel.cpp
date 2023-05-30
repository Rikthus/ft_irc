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
	mClientList.insert(std::pair<int, Client *>(clientSockfd, &clientData));
	if (mCapped)
		mMaxCapacity++;
	for (std::vector<int>::iterator it; it != mInvited.end(); it++)
	{
		if (*it == clientSockfd)
			mInvited.erase(it);
	}
}

void	Channel::addInvitation(int isInvited)
{
	mInvited.push_back(isInvited);
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
		mPwd = "\0";
	}
	mClientList.insert(std::pair<int, Client*>(mAdmin, &creatorData));
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

//////**********************************//////
//////				PRIVATE				//////
//////**********************************//////

