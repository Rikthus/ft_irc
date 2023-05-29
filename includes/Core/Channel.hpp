/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxperei <maxperei@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 18:11:17 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/29 14:50:14 by maxperei         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "ft_irc.hpp"
# include "Client.hpp"

# define NO_PWD "\0"

class Channel
{
	typedef std::map<int, Client *> ClientData;

	public:

		bool	findInvite(int clientSockfd) const;
		bool	checkPwd(std::string key) const;
		bool	checkSpace(void) const;
		void	addClient(int clientSockfd, Client &clientData);

		Channel(std::string chanName, int creatorSockfd, Client &creatorData, bool isPwd, std::string pwd);
		~Channel(void);

	private:
		std::string 		mName;
		int					mAdmin;
		ClientData  		mClientList;
		std::vector<int>	mOperators;
		std::vector<int>	mInvited;
		std::string			mPwd;

		bool		mPassProtected;
		bool		mInviteOnly;
		bool		mCapped;
		int			mMaxCapacity;
};

typedef std::map<std::string, Channel>::iterator channelIt;
typedef std::map<std::string, Channel>::const_iterator constChannelIt;
