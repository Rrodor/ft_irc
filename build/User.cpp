/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:26:34 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/13 19:59:41 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

User::User(int & fd)
{
	this->fd = fd;
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
