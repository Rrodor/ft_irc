/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 22:02:28 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/06 12:40:24 by babreton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

Channel::Channel(): _name("default"), _topic("default")
{
}

Channel::Channel(std::string name, std::string topic) : _name(name), _topic(topic)
{
}

Channel::~Channel()
{
}

std::string		Channel::getName()
{
	return _name;
}

std::string		Channel::getTopic()
{
	return _topic;
}

void			Channel::setName(std::string name)
{
	_name = name;
}

void			Channel::setTopic(std::string topic)
{
	_topic = topic;
}

void			Channel::addUser(User user)
{
	_users.insert(std::pair<std::string, User>(user.getName(), user));
}

void			Channel::removeUser(User user)
{
	_users.erase(user.getName());
}
