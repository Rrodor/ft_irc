/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:36:08 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/16 16:38:53 by babreton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

Channel::Channel(std::string name)
{
	this->name = name;
	this->topic = "";
	std::time_t t = std::time(0);
	std::ostringstream oss;
	oss << t;
	this->lastTopicUpdate = oss.str();
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

void	Channel::printChannelUsers() const
{
	std::string ulist = JOIN;
	ulist += "[#" + this->name + "]" + "[USERS_LIST] > ";
	for (std::vector<User *>::const_iterator it2 = this->users.begin(); it2 != this->users.end(); ++it2)
	{
		ulist += "|" + (*it2)->nickname + "| ";
	}
	std::cout << ulist << std::endl;
	std::string olist = JOIN;
	olist += "[#" + this->name + "]" + "[OPERATORS_LIST] > ";
	for (std::vector<User *>::const_iterator it3 = this->operators.begin(); it3 != this->operators.end(); ++it3)
	{
		olist += "|" + (*it3)->nickname + "| ";
	}
	std::cout << olist << std::endl;
}

void	Channel::channelSendLoop(std::string message, int & sFd, Server * server, int sendToHim)
{
	std::vector<User *>::iterator	it = this->users.begin();
	std::vector<User *>::iterator	ite = this->users.end();

	while (it != ite)
	{
		if (sFd != (*it)->fd || (sFd == (*it)->fd && sendToHim == 1))
		{
			send((*it)->fd, message.c_str(), message.length(), 0);
			send_log((*it)->fd, message.c_str(), server);
		}
		it++;
	}
	it = this->operators.begin();
	ite = this->operators.end();
	while (it != ite)
	{
		if (sFd != (*it)->fd || (sFd == (*it)->fd && sendToHim == 1))
		{
			send((*it)->fd, message.c_str(), message.length(), 0);
			send_log((*it)->fd, message.c_str(), server);
		}
		it++;
	}
}

void	Channel::deleteChannelUser(User * user, Server * server)
{
	std::vector<User *>::iterator	it = this->users.begin();
	std::vector<User *>::iterator	ite = this->users.end();
	
	if (this->isOpInChannel(user))
	{
		it = this->operators.begin();
		ite = this->operators.end();
	}

	while (it != ite)
	{
		if (user == (*it))
		{
			if (this->isInChannel(user))
			{
				this->users.erase(it);
				std::cout << QUIT << "Erasing user " << user->nickname << " from #" << this->name << RESET << std::endl;
			}
			else if (this->isOpInChannel(user))
			{
				this->operators.erase(it);
				std::cout << QUIT << "Erasing operator " << user->nickname << " from #" << this->name << RESET << std::endl;
			}
		}
		it++;
	}
	if (this->operators.empty())
		this->allocNewOp(server);
}

void	Channel::allocNewOp(Server * server)
{
	std::cout << YELLOW << "#" << this->name << " is empty, allocating a new operator..." << RESET << std::endl;
	User *	newOp = this->users.front();
	this->users.erase(this->users.begin());
	this->operators.push_back(newOp);
	std::cout << YELLOW << "Adding " << newOp->nickname << " as new channel operator." << RESET << std::endl;
	irc_names(this, newOp, server);
}