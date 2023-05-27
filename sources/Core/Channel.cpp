#include "../../includes/Core/Channel.hpp"

//////**********************************//////
//////				PUBLIC				//////
//////**********************************//////

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

