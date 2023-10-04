/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cparras <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:42:13 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/04 17:50:16 by cparras          ###   ########.fr       */
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

void	displayHelp(User &user)
{
	send(user.getFd(), "List of commands:\n", 18, 0);
	send(user.getFd(), "/nick: change your nickname\n", 28, 0);
	send(user.getFd(), "/quit: quit the server\n", 24, 0);
}

void	getorder(char* buffer, User &user)
{
	int	valread;
	if (strcmp(buffer, "/nick") == 0)
	{
		send(user.getFd(), "Enter a nickname: ", 19, 0);
		bzero(buffer, BUFFSIZE);
		valread = read(user.getFd(), buffer, BUFFSIZE);
		if (valread == 1)
			user.setHasNickname(false);
		else
		{
			buffer[strlen(buffer) - 1] = '\0';
			
			user.setNickname(buffer);
			bzero(buffer, BUFFSIZE);
		}
	}
	else if (strcmp(buffer, "/quit") == 0)
	{
		send(user.getFd(), "Quit server: ", 14, 0);
		close(user.getFd());
		user.setFd(-1);
	}
	else if (strcmp(buffer, "/help") == 0)
		displayHelp(user);
	else
		send(user.getFd(), "Unknown command\n/help for list of commands\n", 44, 0);
}

int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char	buffer[BUFFSIZE + 1];

	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv [port] [password]"<<std::endl;
		return 0;
	}
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(std::atoi(argv[1]));

	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	User user = init(new_socket, argv[2]);
	while (1)
	{

		send(new_socket, "> ", 2, 0);
		valread = read(new_socket, buffer, BUFFSIZE);
		if (buffer[0] == '/')
		{
			buffer[valread - 1] = '\0';
			getorder(buffer, user);
		}
		else
		{
			if (user.getNickname().empty())
				std::cout << user.getName() << " : " << buffer;
			else
				std::cout << user.getNickname() << " : " << buffer;
		}
		bzero(buffer, BUFFSIZE);
	}

	close(new_socket);
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
