/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 22:02:28 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/11 17:37:03 by rrodor           ###   ########.fr       */
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

std::string		Channel::getName() const
{
	return _name;
}

std::string		Channel::getTopic() const
{
	return _topic;
}

std::map<int, User>		Channel::getUsers() const
{
	return _users;
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
	_users.insert(std::pair<int, User>(user.getFd(), user));
}

void			Channel::removeUser(User user)
{
	_users.erase(user.getFd());
}

std::ostream &	operator<<(std::ostream & o, Channel const &rSym)
{
	o << rSym.getName();
	return o;
}