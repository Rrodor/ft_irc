/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:56:20 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/16 18:49:15 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

User & User::operator=(const User & ref)
{
	this->_fd = ref.getFd();
	this->_name = ref.getName();
	this->_nickname = ref.getNickname();
	this->_hasNickname = ref.getHasNickname();
	this->_channel = ref.getChannel();
	return *this;
}

User::User(int & fd, Channel & channel, Server & server, int & fdsId) : _fd(fd), _fdsId(fdsId), _hasNickname(false), _channel(channel), _server(server)
{
	this->initName();
	this->initNickname();
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

int			User::getFd() const
{
	return this->_fd;
}

Channel		User::getChannel() const
{
	return this->_channel;
}

int			User::getFdsId() const
{
	return this->_fdsId;
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

void		User::setChannel(Channel &channel)
{
	this->_channel = channel;
	channel.addUser(*this);
}

void		User::initName()
{
	int		valread;
	char	buffer[BUFFSIZE + 1];

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
	this->_name = buffer;
	bzero(buffer, BUFFSIZE);
}

void		User::initNickname()
{
	char	buffer[BUFFSIZE + 1];
	int		valread;

	send(this->_fd, "Enter a nickname: ", 19, 0);
	valread = read(this->_fd, buffer, BUFFSIZE);
	this->_hasNickname = true;
	buffer[valread - 1] = '\0';
	this->_nickname = buffer;
	bzero(buffer, BUFFSIZE);
}

void 		User::sendMessage(std::string message, int fd)
{
	std::map<int, std::pair<User, Channel> >::iterator	it;
	int													i;

	for (i = 0, it = this->_server.getUsersList().begin(); i < this->_server.getUsersList().size(); i++)
	{
		if (it->first != fd) {
			send(it->first, message.c_str(), message.length(), 0);
			send(it->first, "> ", 2, 0);
		}
		if (i + 1 < this->_server.getUsersList().size())
			it++;
	}
}

void	User::deleteUser() {
	this->_server.destroyFd(this->_fd);
	this->_server.fds[this->_fdsId].fd = -1;
	close(this->_fd);
	this->_fd = -1;
}