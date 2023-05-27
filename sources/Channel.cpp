#include "../includes/Channel.hpp"

//////**********************************//////
//////				PUBLIC				//////
//////**********************************//////

//////////////////////////
//		BUILDERS		//
//////////////////////////

Channel::Channel(std::string chanName, int creatorSockfd, Client &creatorData, bool isPwd, std::string pwd): mName(chanName), mAdmin(creatorSockfd)
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
	mClientList.insert(std::pair<int, Client &>(creatorSockfd, creatorData));
	mOperators.push_back(creatorSockfd);
}

Channel::~Channel(void)	{}

//////**********************************//////
//////				PRIVATE				//////
//////**********************************//////

