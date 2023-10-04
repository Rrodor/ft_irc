/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Init.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cparras <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 16:17:50 by cparras           #+#    #+#             */
/*   Updated: 2023/10/04 18:06:36 by cparras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include "../User.hpp"

#define BUFFSIZE 1024

bool	checkEmptyName(char *src)
{
	int i = 0;
	while (src[i])
	{
		if (src[i] != ' ' && src[i] != '\t' && src[i] != '\n' && src[i] != '\0')
			return true;
		i++;
	}
	return false;
}

void	initNickname(User &user, int new_socket)
{
	char	buffer[BUFFSIZE + 1];
	int		valread;

	send(new_socket, "Enter a nickname: ", 19, 0);
	valread = read(new_socket, buffer, BUFFSIZE);
	user.setHasNickname(true);
	buffer[valread - 1] = '\0';
	user.setNickname(buffer);
	bzero(buffer, BUFFSIZE);
}

void	initName(User &user, int new_socket)
{
	int		valread;
	char	buffer[BUFFSIZE + 1];

	while (true)
	{
		send(new_socket, "Enter a name: ", 15, 0);
		valread = read(new_socket, buffer, BUFFSIZE);
		if (valread != 1 && checkEmptyName(buffer) == true)
			false ;
	}
	buffer[valread - 1] = '\0';
	user.setName(buffer);
	bzero(buffer, BUFFSIZE);
}

void	connection(int fd, std::string password)
{
	char	buffer[BUFFSIZE + 1];
	int		valread;
	bool	connected = false;

	while (!connected)
	{
		send(fd, "Enter the password: ", 21, 0);
		valread = read(fd, buffer, BUFFSIZE);
		buffer[valread - 1] = '\0';
		if (password.compare(buffer) == 0)
		{
			send(fd, "Welcome to the server\n", 23, 0);
			connected = true;
		}
		else
		{
			bzero(buffer, BUFFSIZE);
			send(fd, "Wrong password.\n", 17, 0);
		}
	}
}

User	init(int new_socket, const char *password)
{
	User	user(new_socket);
	user.setName("");

	connection(user.getFd(), password);
	if (user.getFd() == -1)
	{
		user.~User();
		exit;
	}
	if (user.getName().empty())
		initName(user, new_socket);
	if (user.getNickname().empty() && !user.getHasNickname())
		initNickname(user, new_socket);
	return user;
}