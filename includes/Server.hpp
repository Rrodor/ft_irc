/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:57:48 by babreton          #+#    #+#             */
/*   Updated: 2023/10/14 19:00:00 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <netinet/in.h>

class Server {
	public:
		Server(const char * port);
		~Server();

		int &											getServerSocket();
		std::map<std::string, Channel *> &				getChannelsList();
		User *											getUserByFd(int fd);
		User *											getUserByFdsId(int fdsId);
		std::map<int, std::pair<User *, Channel *> > &	getUsersList();
		int												getChannelSize(Channel * channel) const;
		int												getChannelsListSize() const;

		int			newUser(int & fd, const char * password, int & fdsId);
		void		changeUserChannel(std::string channelName, std::string buffer, User * user);
		void		printConnectedUsers() const;
		void		deleteUser(int fd, std::string name, int fdsId);

		int				current_size;
		struct pollfd	fds[200];
	private:
		void	_initServer();

		int												_serverSocket;
		struct sockaddr_in								_adress;
		const int										_port;
		Channel *										_general;
		std::map<std::string, Channel *>				_channels;
		std::map<int, std::pair<User *, Channel *> >	_connectedUsers; 
};

#endif
