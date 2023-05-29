/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxperei <maxperei@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 17:46:31 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/29 14:35:30 by maxperei         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "ft_irc.hpp"

class Client
{        
        public:
            Client();
            ~Client();
            void appendToBuffer(const char *data, int size);
            bool extractMessageFromBuffer(std::string &message);

            sockaddr_in &getStruct() {return ClientAddr;}
            socklen_t   &getClilen() {return clilen;}
            in_addr     getAddr() {return ClientAddr.sin_addr;}
            
            int         getPort() {return ClientAddr.sin_port;}
            int         getFd() {return clifd;}
            bool        getAuthentication() {return isAuthenticated;}
            bool        getRegistration() {return isRegistered;}
            
            void        setClifd(int &fd) {this->clifd = fd;}
            void        setAuthentication() {this->isAuthenticated = true;}
            void        setRegistration() {this->isRegistered = true;}
            void        setNickname(std::string name) {this->nickname = name;}
            void        setUsername(std::string name) {this->username = name;}
            
            std::string getNickname() {return nickname;}
            std::string getUsername() {return username;}
            
        private:
            std::string         nickname;
            std::string         username;
            std::string         buffer;
            bool                isAuthenticated;
            bool                isRegistered;
            struct sockaddr_in  ClientAddr;
            socklen_t           clilen;
            int                 clifd;
};

typedef std::map<int, Client>::iterator clientIt;
typedef std::map<int, Client>::const_iterator constClientIt;

// void 	error(const char *msg);
// void	authenticateClient(std::string msg, int fd, ft::Client &Client);
// void	sendMessage(int fd, std::string message);
// void	registerClientsNick(std::string msg, int fd, ft::Client &Client, std::map<int,ft::Client> Clients);
// void	registerClientsUser(std::string msg, int fd, ft::Client &Client, std::map<int,ft::Client> Clients);

// bool	checkDuplicateNick(std::map<int, ft::Client> Clients, std::string nickname);
// bool	checkDuplicateUser(std::map<int, ft::Client> Clients, std::string username);
// bool	checkCharacter(char character);
// bool	checkCharactersValidity(std::string name);

// void    newClient(std::map<int, ft::Client> &Clients, int &max_fd, fd_set &readfds, int sockfd);
// void	registerClientsUser(std::string msg, int fd, ft::Client &Client, std::map<int,ft::Client> Clients);
// void	registerClientsNick(std::string msg, int fd, ft::Client &Client, std::map<int,ft::Client> Clients);