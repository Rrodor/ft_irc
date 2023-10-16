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

/*User::User() : _fd(0), _hasNickname(false)
{
	Channel channel("default", "default");
	_channel = channel;
}*/

User & User::operator=(const User & ref)
{
	_fd = ref.getFd();
	_name = ref.getName();
	_nickname = ref.getNickname();
	_hasNickname = ref.getHasNickname();
	_channel = ref.getChannel();
	return *this;
}

User::User(int & fd, Channel &channel) : _fd(fd), _hasNickname(false), _channel(channel)
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

void		User::setChannel(Channel &channel)
{
	_channel = channel;
	channel.addUser(*this);
}

void User::sendMessage(std::string message) const
{
    std::map<std::string, User>::const_iterator it;
    for (it = _channel.getUsers().begin(); it != _channel.getUsers().end(); it++)
    {
        if (it->first == _name)
            continue;
        send(it->second.getFd(), message.c_str(), message.length(), 0);
    }
}
