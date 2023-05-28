/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 18:11:17 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/27 14:24:25 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"

class Channel
{
    typedef std::map<int, Client *> ClientData;

    public:
        Channel();
        ~Channel();

        std::string getName() const {return name;} // putain je sais pas pk mais si je le passe pas en const ca compile pas
    
    private:
        ClientData  clients;
        std::string name;
        
        //ft::Client  operator;
};
