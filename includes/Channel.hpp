/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tulipe <tulipe@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 18:11:17 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/27 13:10:07 by tulipe           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"

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
