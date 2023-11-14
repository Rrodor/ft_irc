/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:36:08 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/13 20:02:17 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

Channel::Channel(std::string name)
{
	this->name = name;
}

Channel::~Channel()
{
}

bool	Channel::isInChannel(User *user)
{
	if (!user)
		return false;

	std::vector<User *>::iterator		it = this->users.begin();
	std::vector<User *>::iterator		ite = this->users.end();

	while (it != ite)
	{
		if (user == *it)
			return true;
		it++;
	}
	return false;
}

bool	Channel::isOpInChannel(User *user)
{
	if (!user)
		return false;

	std::vector<User *>::iterator		it = this->operators.begin();
	std::vector<User *>::iterator		ite = this->operators.end();

	while (it != ite)
	{
		if (user == *it)
			return true;
		it++;
	}
	return false;
}