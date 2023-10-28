/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cparras <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:56:20 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/23 14:20:41 by cparras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

User & User::operator=(User & rhs)
{
	this->_fd = rhs.getFd();
	this->_name = rhs.getName();
	this->_nickname = rhs.getNickname();
	this->_hasNickname = rhs.getHasNickname();
	this->_channel = rhs.getChannel();
	return *this;
}

User::User(int & fd, Channel * channel, Server & server, int & fdsId) : _fd(fd), _fdsId(fdsId), _hasNickname(false), _channel(channel), _server(server)
{
	this->initName();
	this->initNickname();
	std::cout << GREEN << "[STATUS] : New user named \"" << this->getName() << "\" connected with id " << this->getFd() << "." << RESET << std::endl;
}

User::~User()
{
}

std::string User::getName() const
{
	return this->_name;
}

std::string User::getNickname() const
{
	return this->_nickname;
}

bool		User::getHasNickname() const
{
	return this->_hasNickname;
}

int &		User::getFd()
{
	return this->_fd;
}

Channel *	User::getChannel() const
{
	return	this->_channel;
}

int &		User::getFdsId()
{
	return this->_fdsId;
}

Server &	User::getServer() const
{
	return this->_server;
}

void		User::setName(std::string name)
{
	this->_name = name;
}

void		User::setNickname(std::string nickname)
{
	this->_nickname = nickname;
}

void		User::setFd(int fd)
{
	this->_fd = fd;
}

void		User::setHasNickname(bool hasNickname)
{
	this->_hasNickname = hasNickname;
}

void		User::setChannel(Channel * channel)
{
	this->_channel = channel;
}

void		User::initName()
{
	int				valread;
	char			buffer[BUFFSIZE + 1];
	std::string		name;

	while (1)
	{
		bzero(buffer, BUFFSIZE);
		send(this->_fd, "Enter a name: ", 15, 0);
		valread = read(this->_fd, buffer, sizeof(buffer));
		if (valread == -1)
		{
			std::cout << "read failed" << std::endl;
			break;
		}
		if (checkEmptyName(buffer) == true)
			break ;
		buffer[valread - 1] = '\0';
		bzero(buffer, BUFFSIZE);
	}
	buffer[valread - 1] = '\0';
	name = trimName(buffer, 0);
	this->_name = name;
	bzero(buffer, BUFFSIZE);
}

void		User::initNickname()
{
	char		buffer[BUFFSIZE + 1];
	int			valread;
	std::string	name;

	send(this->_fd, "Enter a nickname: ", 19, 0);
	valread = read(this->_fd, buffer, BUFFSIZE);
	this->_hasNickname = true;
	buffer[valread - 1] = '\0';
	name = trimName(buffer, 1);
	this->_nickname = name;
	bzero(buffer, BUFFSIZE);
}

void 		User::sendMessage(std::string message, int fd, Channel * channel)
{
	std::string														channelName = channel->getName();
	std::map<int, std::pair<User *, Channel *> >::const_iterator	it = this->getServer().getUsersList().begin();
	std::map<int, std::pair<User *, Channel *> >::const_iterator	ite = this->getServer().getUsersList().end();

	std::cout << "Channe id : " << channel->getChannelId() << std::endl;
	while (it != ite)
	{
		if (it->first != fd && it->second.second->getName() == channelName)
		{
			send(it->first, message.c_str(), message.length(), 0);
			send(it->first, "> ", 2, 0);
		}
		it++;
	}
}
