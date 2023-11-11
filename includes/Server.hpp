/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 15:41:33 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/11 18:41:35 by rrodor           ###   ########.fr       */
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

		int		getServerSocket() const;
		void	newUser(int & fd);

		class WrongPasswordException : public std::exception
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
