/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tulipe <tulipe@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 18:11:17 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/27 23:20:31 by tulipe           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ft_irc.hpp"
# include "Client.hpp"

class Channel
{
	typedef std::map<int, Client &> ClientData;

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

#endif
