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

	// Init Channels //
	Channel * general = new Channel("general", "general");
	this->_channels.insert(std::pair<std::string, Channel *>("general", general));

	// Init fds //
	memset(this->fds, 0 , sizeof(this->fds));
	this->fds[0].fd = this->_serverSocket;
	this->fds[0].events = POLLIN;
}

int	Server::getServerSocket() const	
{
	return this->_serverSocket;
}

std::map<int, std::pair<User *, Channel *> > &	Server::getUsersList()
{
	return this->_connectedUsers;
}

std::map<std::string, Channel *> &				Server::getChannelsList()
{
	return this->_channels;
}

User *	Server::getUserByFd(int fd)
{
	std::map<int, std::pair<User *, Channel *> >::iterator	it;

	for (it = this->_connectedUsers.begin(); it != this->_connectedUsers.end(); it++)
		if (it->first == fd)
			return it->second.first;
	return it->second.first;
}

User *	Server::getUserByFdsId(int fdsId)
{
	std::map<int, std::pair<User *, Channel *> >::iterator	it;

	for (it = this->_connectedUsers.begin(); it != this->_connectedUsers.end(); it++)
		if (it->second.first->getFdsId() == fdsId)
			return it->second.first;
	return it->second.first;
}

void	Server::_initServer() {
	int	opt = 1;
	int	addrlen = sizeof(this->_adress);

	if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("[ERROR] : can't create a socket, please check your permissions or cpu arch.");

	if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("[ERROR] : can't set options of socket, please check your permissions or cpu arch.");

	if (ioctl(this->_serverSocket, FIONBIO, (char *)&opt))
		throw std::runtime_error("[ERROR] : can't recover device information, please check your permissions or cpu arch.");

	this->_adress.sin_family = AF_INET;
	this->_adress.sin_addr.s_addr = inet_addr("127.0.0.1");
	this->_adress.sin_port = htons(this->_port);

	if (bind(this->_serverSocket, (struct sockaddr*)&this->_adress, sizeof(this->_adress)) < 0)
		throw std::runtime_error("[ERROR] : can't get local adress informations, bind failed, please check your permissions or cpu arch.");

	if (listen(this->_serverSocket, 3) < 0)
		throw std::runtime_error("[ERROR] : can't listen on the adress, please check your permissions or cpu arch.");

}

int		Server::newUser(int & fd, const char * password, int & fdsId)
{
	if (fd == -1)
	{
		std::cerr << "[ERROR] : Fatal error client." << std::endl;
		return 1;
	}

	this->fds[fdsId].fd = fd;
	this->fds[fdsId].events = POLLIN;
		
	connectToClient(fd, password);
	User * user = new User(fd, this->_channels["general"], *this, fdsId);

	this->_connectedUsers.insert(std::make_pair(fd, std::make_pair(user, this->_channels["general"])));
	send(fd, "> ", 2, 0);

	return 0;
}

int		Server::getChannelSize(Channel * channel) const
{
	std::map<int, std::pair<User *, Channel *> >::const_iterator	it = this->_connectedUsers.begin();
	std::map<int, std::pair<User *, Channel *> >::const_iterator	ite = this->_connectedUsers.end();
	
	int	i = 0;

	while (it != ite)
	{
		if (it->second.second->getName() == channel->getName())
			i++;
		it++;
	}

	return i;
}

void	Server::changeUserChannel(std::string channelName, std::string buffer, User * user)
{
	std::map<std::string, Channel *>::iterator	it = this->_channels.begin();
	std::map<std::string, Channel *>::iterator	ite = this->_channels.end();

	Channel *	channel = NULL;
	
	// check if channel exist
	while (it != ite)
	{
		if (channelName == it->second->getName())
			channel = it->second;
		it++;
	}

	// only delete empty channel, but not the main one
	if (user->getChannel()->getName() != "général" || this->getChannelSize(user->getChannel()) == 1)
		delete user->getChannel();
	
	if (channel == NULL)
	{
		Channel * newChannel = new Channel(channelName, buffer);
		this->_channels.insert(std::pair<std::string, Channel *>(channelName, newChannel));
	}
	
	// switch channel
	Channel * newChannel = this->_channels[channelName];
	this->_connectedUsers[user->getFd()].first->setChannel(newChannel);
}

void	Server::printConnectedUsers() const
{
	std::map<int, std::pair<User *, Channel *> >::const_iterator	it;

	for (it = this->_connectedUsers.begin(); it != this->_connectedUsers.end(); it++)
	{
		std::cout << "User : " << it->second.first->getName() << " connected with socket ";
		std::cout << it->first << " and fdsId " << it->second.first->getFdsId() << ". Channel : ";
		std::cout << it->second.second->getName() << std::endl;
	}
}

void	Server::deleteUser(int fd, std::string name, int fdsId)
{
	std::cout << RED << "[STATUS] : deleting user id " << fd << " named " << name << "." << RESET << std::endl;
	close(this->fds[fdsId].fd);
	this->fds[fdsId].fd = -1;
	delete this->_connectedUsers.find(fd)->second.first;
	this->_connectedUsers.erase(fd);
}

Server::~Server()
{
	shutdown(this->_serverSocket, SHUT_RDWR);
}
