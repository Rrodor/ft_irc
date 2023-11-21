/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 15:41:33 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/21 11:56:49 by babreton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include "ft_irc.hpp"

class User;
class Channel;

class Server
{
	public:
		std::vector<struct pollfd>	fds;
		int							current_size;
		std::vector<User *>			users;
		std::vector<Channel *>		channels;
		std::string					password;

		Server(const char * port, std::string password);
		~Server();

		int									getServerSocket() const;
		std::vector<Channel *>::iterator	getChannelByName(std::string name);
		std::vector<User *>::iterator		getUserByName(std::string name);
		std::vector<User *>::iterator		getUserByFd(int & fd);

		void		newUser(int & fd);
		void		passUser(int & fd, const char * message, User * user);
		void		capUser(int & fd, const char * message, User * user);
		void		nickUser(int & fd, const char * message, User * user);
		void		userUser(int & fd, const char * message, User * user);
		void		printServerChannels(std::string name) const;
		void		checkChannel();
		void		deleteUser(int & fd);
		bool		haveN(std::string message) const;
		std::string	writeLoop(int & fd, std::string buffer);
		int			checkNick(int & fd, std::string nickname);

		class PassException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class CapException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class NickException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class UserException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
	private:
		void	_initServer();
		int									_serverSocket;
		struct sockaddr_in					_adress;
		const int							_port;
		//std::vector<User *>					_users;
};

#endif
