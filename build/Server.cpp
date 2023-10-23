/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 13:35:22 by babreton          #+#    #+#             */
/*   Updated: 2023/10/14 19:09:32 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

Server::Server(const char * port) : current_size(0), _port(std::atoi(port))
{
	this->_initServer();
}

int	Server::getServer() const
{
	return (this->_server);
}

std::map<int, std::pair<User, Channel> >	Server::getUsersList() const
{
	return this->_connectedUsers;
}

std::map<std::string, Channel> &			Server::getChannels()
{
	return this->_channels;
}

User &	Server::getUserByFd(int fd)
{
	std::map<int, std::pair<User, Channel> >::iterator	it;

	for (it = this->_connectedUsers.begin(); it != this->_connectedUsers.end(); it++)
		if (it->first == fd)
			break;
	if (it == this->_connectedUsers.end())
		exit(1);
	return it->second.first;
}

void	Server::_initServer() {
	int	opt = 1;
	int	addrlen = sizeof(this->_adress);

	if ((this->_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(this->_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	if (ioctl(this->_server, FIONBIO, (char *)&opt))
	{
		perror("ioctl() failed");
		exit(EXIT_FAILURE);
	}
	this->_adress.sin_family = AF_INET;
	this->_adress.sin_addr.s_addr = inet_addr("127.0.0.1");
	this->_adress.sin_port = htons(this->_port);

	if (bind(this->_server, (struct sockaddr*)&this->_adress, sizeof(this->_adress)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(this->_server, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
}

void	Server::newUser(int & fd, const char * password, Channel & channel, int fdsId)
{
	if (fd == -1)
		exit(0);
		
	connectToClient(fd, password);

	User						user(fd, channel, *this, fdsId);

	this->_connectedUsers.insert(std::make_pair(fd, std::make_pair(user, channel)));
	send(fd, "> ", 2, 0);
}

void	Server::printConnectedUsers() const
{
	std::map<int, std::pair<User, Channel> >::const_iterator	it;

	for (it = this->_connectedUsers.begin(); it != this->_connectedUsers.end(); it++)
	{
		std::cout << "User : " << it->second.first.getName() << " connected with socket ";
		std::cout << it->first << ". Channel : " << it->second.second.getName() << std::endl;
	}
}

void	Server::destroyFd(int fd, std::string name)
{
	std::cout << RED << "[STATUS] : deleting user id " << fd << " named " << name << "." << RESET << std::endl;
	this->_connectedUsers.erase(fd);
}

Server::~Server()
{
	shutdown(this->_server, SHUT_RDWR);
}
