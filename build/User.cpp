/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:56:20 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/11 17:40:57 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

User::User(int fd) : _fd(fd), _hasNickname(false)
{
}

User::~User()
{
}

std::string User::getName() const
{
	return _name;
}

std::string User::getNickname() const
{
	return _nickname;
}

bool		User::getHasNickname() const
{
	return _hasNickname;
}

int			User::getFd() const
{
	return _fd;
}

Channel		User::getChannel() const
{
	return _channel;
}

void		User::setName(std::string name)
{
	_name = name;
}

void		User::setNickname(std::string nickname)
{
	_nickname = nickname;
}

void		User::setFd(int fd)
{
	_fd = fd;
}

void		User::setHasNickname(bool hasNickname)
{
	_hasNickname = hasNickname;
}

void		User::setChannel(Channel channel)
{
	_channel = channel;
}

void		User::sendMessage(std::string message) const
{
	for (std::map<std::string, User>::iterator it = _channel.getUsers().begin(); it != this->_channel.getUsers().end(); it++)
	{
		if (it->first == this->getName())
			continue;
		send(it->second.getFd(), message.c_str(), message.length(), 0);
	}
}
