/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:42:13 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/03 19:58:39 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Server side C/C++ program to demonstrate Socket
// programming
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

void	connection(int fd, std::string password)
{
	char	buffer[BUFFSIZE + 1];
	int		valread;
	bool	connected = false;

	while (!connected)
	{
		send(fd, "Enter the password: ", 20, 0);
		valread = read(fd, buffer, BUFFSIZE);
		buffer[valread - 1] = '\0';
		if (password.compare(buffer) == 0)
		{
			send(fd, "Welcome to the server\n", 22, 0);
			connected = true;
		}
		else
		{
			bzero(buffer, BUFFSIZE);
			send(fd, "Wrong password.\n", 17, 0);
		}
	}
}

void	getorder(char* buffer, User &user)
{
	if (strcmp(buffer, "/nick") == 0)
	{
		send(user.getFd(), "Enter a nickname : ", 20, 0);
		bzero(buffer, BUFFSIZE);
		read(user.getFd(), buffer, BUFFSIZE);
		buffer[strlen(buffer) - 1] = '\0';
		user.setNickname(buffer);
	}
	else if (strcmp(buffer, "/quit") == 0)
	{
		send(user.getFd(), "Quit server : ", 15, 0);
		close(user.getFd());
		user.setFd(-1);
	}
	else
		send(user.getFd(), "Unknown command\n", 18, 0);
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
		std::cout << "missing argument" <<std::endl;
		return 0;
	}
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(std::atoi(argv[1]));

	// Forcefully attaching socket to the port 8080
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
	User user(new_socket);
	user.setName("");
	connection(user.getFd(), argv[2]);
	while (1)
	{
		if (user.getFd() == -1)
		{
			std::cout << "test" << std::endl;
			break;
		}
		if (user.getName().empty())
		{
			send(new_socket, "Enter a name :", 15, 0);
			valread = read(new_socket, buffer, BUFFSIZE);
			buffer[valread - 1] = '\0';
			user.setName(buffer);
			bzero(buffer, BUFFSIZE);
		}
		if (user.getNickname().empty())
		{
			send(new_socket, "Enter a nickname : ", 20, 0);
			valread = read(new_socket, buffer, BUFFSIZE);
			buffer[valread - 1] = '\0';
			user.setNickname(buffer);
			bzero(buffer, BUFFSIZE);
		}
		send(new_socket, "> ", 2, 0);
		valread = read(new_socket, buffer, BUFFSIZE);
		buffer[valread - 1] = '\0';
		if (buffer[0] == '/')
			getorder(buffer, user);
		else
			std::cout << user.getNickname() << " : " << buffer;
		bzero(buffer, BUFFSIZE);
	}

	// closing the connected socket
	close(new_socket);
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
