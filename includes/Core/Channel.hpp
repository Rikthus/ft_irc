/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tulipe <tulipe@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 18:11:17 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/30 12:27:56 by tulipe           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "ft_irc.hpp"
# include "Client.hpp"
# include "RPL.hpp"

# define NO_PWD "\0"
# define TOPIC_PROTECTED 0
# define PASS_PROTECTED 1
# define INVITE_ONLY 2
# define CAPPED 3
# define TOPIC_SET 4

# define MAX_CHAN_CAPACITY 30

class Channel
{
	typedef std::map<int, Client *> ClientData;

	public:
		

		bool	findInvite(int clientSockfd) const;
		bool	checkPwd(std::string key) const;
		bool	checkSpace(void) const;
		void	addClient(int clientSockfd, Client &clientData);

		void	rplTopic(void);

		bool	getTopicProtected(void) const;
		bool	getPassProtected(void) const;
		bool	getInviteOnly(void) const;
		bool	getCapped(void) const;
		bool	getTopicSet(void) const;
		std::string	getTopic(void) const;

		void	setTopicProtected(bool mode);
		void	setPass(bool mode, std::string newPass);
		void	setInviteOnly(bool mode);
		void	setCapped(bool mode, unsigned int newSize);
		void	setTopic(bool mode, std::string newTopic);

		Channel(std::string chanName, int creatorSockfd, Client &creatorData, bool isPwd, std::string pwd);
		~Channel(void);

	private:
		std::string 		mName;
		int					mAdmin;
		ClientData  		mClientList;
		std::vector<int>	mOperators;
		std::vector<int>	mInvited;
		std::string			mPwd;
		std::string			mTopic;

		bool		mTopicProtected;
		bool		mPassProtected;
		bool		mInviteOnly;
		bool		mCapped;
		bool		mTopicSet;
		unsigned int	mMaxCapacity;
};

typedef std::map<std::string, Channel>::iterator channelIt;
typedef std::map<std::string, Channel>::const_iterator constChannelIt;
