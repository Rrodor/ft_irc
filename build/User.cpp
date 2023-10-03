/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:56:20 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/03 16:32:27 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../User.hpp"

User::User(int fd) : _fd(fd)
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
