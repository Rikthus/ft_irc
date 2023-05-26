/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 18:11:17 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/10 19:05:45 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"

namespace NAMESPACE_IRC
{
    class Channel
    {
        typedef std::map<int, Client *> ClientData;

        public:

        private:
            ClientData  clients;
            std::string name;
            //ft::Client  operator;
    }; 
}
