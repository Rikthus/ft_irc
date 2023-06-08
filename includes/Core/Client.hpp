/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 17:46:31 by eavilov           #+#    #+#             */
/*   Updated: 2023/06/08 17:11:49 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "ft_irc.hpp"

class Client
{        
        public:
            Client();
            ~Client();
            void appendToBuffer(char * data, int size);
            bool extractMessageFromBuffer(std::string &message, int fd);

            sockaddr_in &getStruct() {return ClientAddr;}
            socklen_t   &getClilen() {return clilen;}
            in_addr     getAddr() {return ClientAddr.sin_addr;}
            
            int         getPort() {return ClientAddr.sin_port;}
            int         getFd() {return clifd;}
            bool        getAuthentication() {return isAuthenticated;}
            bool        getRegistration() {return isRegistered;}
            bool        getIsBot() {return isBot;}
            bool        hasNick() {return hasNickname;}
            
            void        setClifd(int &fd) {this->clifd = fd;}
            void        setAuthentication() {this->isAuthenticated = true;}
            void        setRegistration() {this->isRegistered = true;}
            void        setNickname(std::string name) {this->nickname = name; this->hasNickname = true;}
            void        setUsername(std::string name) {this->username = name;}
            void        setBotBool() {this->isBot = true;}
            void        addWarns(int amount) {this->warns += amount;}
            
            std::string getNickname() {return nickname;}
            std::string getUsername() {return username;}
            int         getWarnLevel() {return warns;}
            
        private:
            std::string         nickname;
            std::string         username;
            std::string         buffer;
            bool                hasNickname;
            bool                isAuthenticated;
            bool                isRegistered;
            int                 warns;
            bool                isBot;
            struct sockaddr_in  ClientAddr;
            socklen_t           clilen;
            int                 clifd;
};

typedef std::map<int, Client>::iterator clientIt;
typedef std::map<int, Client>::const_iterator constClientIt;
