/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cparras <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:56:20 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/04 15:47:18 by cparras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../User.hpp"

User::User(int fd) : _fd(fd), _hasNickname(false)
{
}

User::~User()
{
}

std::string User::getName()
{
	return _name;
}

std::string User::getNickname()
{
	return _nickname;
}

bool		User::getHasNickname()
{
	return _hasNickname;
}

int			User::getFd()
{
	return _fd;
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
