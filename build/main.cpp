/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 14:03:01 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/06 23:07:13 by rrodor           ###   ########.fr       */
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
		server = new Server(argv[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << "[ERROR] : Error during server creation : " << std::endl;
		std::cerr <<  e.what() << std::endl;
		delete server;
	}

	while (1)
	{
		std::cout << YELLOW << "[STATUS] : Waiting incoming connection ( poll() )..." << RESET << std::endl;
		rc = poll(&(server->fds[0]), fdsId, -1);
		server->current_size = fdsId;
		std::cout << "current_size : " << server->current_size << std::endl;
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
						server->newUser(newSd);
						std::cout << "New user connected with id " << newSd << std::endl;
						//std::cout << GREEN << "[STATUS] : New user connected with id " << newSd << " and join ";
						//std::cout << server->getUserByFdsId(fdsId)->getChannel()->getName() << "[" << server->getUserByFdsId(fdsId)->getChannel()->getChannelId() << "]." << RESET << std::endl;
						fdsId++;
					}
				}
			}
			else if (server->fds[i].fd == -1)
				break;
			else
			{
				std::cout << "test" << std::endl;
				close_conn = FALSE;
				rc = recv(server->fds[i].fd, buffer, sizeof(buffer), 0);
				/*if (rc == 0)
				{
					close(server->getUserByFd(server->fds[i].fd)->getFd());
					close_conn = TRUE;
				}*/
				buffer[rc] = '\0';
				interpretor(buffer, server->fds[i].fd, server);
				if (close_conn)
				{
					close(server->fds[i].fd);
					server->fds[i].fd = -1;
				}
			}
		}
	}
	std::cout << "END OF PROGRAM" << std::endl;
	return 0;
}
