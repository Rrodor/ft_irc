/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 14:03:01 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/21 12:27:47 by babreton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

int main(int argc, char const* argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv [port] [password]"<<std::endl;
		return 0;
	}

	std::string		message;
	char			buffer[BUFFSIZE + 1];
	int				rc = 1, \
					i = 0, \
					fdsId = 1, \
					newSd = 0, \
					close_conn = 0;

	Server			*server;
	try
	{
		server = new Server(argv[1], argv[2]);
	}
	catch(const std::exception& e)
	{
		std::cerr << "[ERROR] : Error during server creation : " << std::endl;
		std::cerr <<  e.what() << std::endl;
		delete server;
	}

	while (1)
	{
		std::cout << YELLOW << "Waiting incoming connection ( poll() )..." << RESET << std::endl;
		rc = poll(&(server->fds[0]), fdsId, -1);
		server->current_size = fdsId;
		for (int i = 0; i < server->current_size; i++)
		{
			if(server->fds[i].revents == 0)
				continue;
			newSd = 0;
			if (server->fds[i].fd == server->getServerSocket())
			{
				while (newSd != -1)
				{
					newSd = accept(server->getServerSocket(), NULL, NULL);
					if (newSd != -1)
					{
						//if (server->newUser(newSd, argv[2], fdsId) == 1)
						//	delete server;
						try
						{
							std::cout << YELLOW << "New user connected with id " << newSd << "." << RESET << std::endl;
							server->newUser(newSd);
							fdsId++;
						}
						catch (const std::exception& e)
						{
							std::cerr << ERROR << "Error during user creation : " << std::endl;
							std::cerr <<  e.what() << RESET << std::endl;
							server->deleteUser(newSd);
							close(newSd);
						}
					}
				}
			}
			else if (server->fds[i].fd == -1)
				break;
			else
			{
				rc = recv(server->fds[i].fd, buffer, sizeof(buffer), 0);
				if (rc == 0)
				{
					std::cout << YELLOW << "Receiving ctrl c from id " << server->fds[i].fd << ", erasing user." << RESET << std::endl;
					close(server->fds[i].fd);
					server->deleteUser(server->fds[i].fd);
					server->checkChannel();
					server->fds[i].fd = -1;
					std::cout << DELETE << "User succesfully deleted" << RESET << std::endl;
					std::cout << DIVIDER << RESET << std::endl;
					continue;
				}
				buffer[rc] = '\0';
				std::string strmess = buffer;
				read_log(server->fds[i].fd, buffer, server);
				if (server->haveN(strmess) == false)
				{
					strmess = server->writeLoop(server->fds[i].fd, strmess);
					interpretor((char*)strmess.c_str(), server->fds[i].fd, server);
					std::cout << DIVIDER << RESET << std::endl;
				}
				else
					interpretor(buffer, server->fds[i].fd, server);
			}
			std::cout << DIVIDER << RESET << std::endl;
		}
	}
	std::cout << "END OF PROGRAM" << std::endl;
	return 0;
}