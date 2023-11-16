/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 14:44:56 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/15 13:52:07 by babreton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

void	send_log(int & fd, const char * message, Server * server)
{
	std::vector<User *>::iterator	it = server->users.begin();
	std::vector<User *>::iterator	ite = server->users.end();
	std::string						name;
	char *							string = strdup(message);

	string = strtok(string, "\r\n");
	while (it != ite && name.empty())
	{
		if (fd == (*it)->fd)
			name = (*it)->nickname;
		it++;
	}
	std::cout << GREEN;
	if (name.empty())
		std::cout << "[" << fd << " - new user] : ";
	else
		std::cout << "[" << fd << " - " << name << "] : ";
	std::cout << string << RESET << std::endl;
	free(string);
}

void	read_log(int & fd, char * buffer, Server * server)
{
	std::vector<User *>::iterator	it = server->users.begin();
	std::vector<User *>::iterator	ite = server->users.end();
	std::string						name;

	buffer = strtok(buffer, "\r\n");
	while (it != ite && name.empty())
	{
		if (fd == (*it)->fd)
			name = (*it)->nickname;
		it++;
	}
	std::cout << CYAN;
	if (name.empty())
		std::cout << "[" << fd << " - new user] : ";
	else
		std::cout << "[" << fd << " - " << name << "] : ";
	std::cout << buffer << RESET << std::endl;
}