/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:26:34 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/21 10:16:59 by babreton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

User::User(int & fd)
{
	this->fd = fd;
	this->conStep = 1;
}

User::~User()
{
}

bool	User::operator==(User & user)
{
	if (this->fd == user.fd)
		return true;
	return false;
}

bool	User::operator!=(User & user)
{
	if (this->fd == user.fd)
		return false;
	return true;
}
