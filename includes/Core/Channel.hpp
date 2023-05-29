/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 18:11:17 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/29 13:32:16 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "ft_irc.hpp"
# include "Client.hpp"

class Channel
{
	typedef std::map<int, Client *> ClientData;

	public:

		Channel(std::string chanName, int creatorSockfd, Client &creatorData, bool isPwd, std::string pwd);
		~Channel(void);

	private:
		std::string 		mName;
		int					mAdmin;
		ClientData  		mClientList;
		std::vector<int>	mOperators;
		std::string			mPwd;

		bool		mPassProtected;
		bool		mInviteOnly;
		bool		mCapped;
		int			mMaxCapacity;
};

typedef std::map<std::string, Channel>::iterator channelIt;
