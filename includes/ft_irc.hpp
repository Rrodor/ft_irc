/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:00:47 by cparras           #+#    #+#             */
/*   Updated: 2023/10/16 15:07:11 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/poll.h>
#include <algorithm>
#include <map>
#include <vector>

#include "Channel.hpp"
#include "User.hpp"
#include "Server.hpp"

#define BUFFSIZE	1024
#define TRUE		1
#define FALSE		0

//init.cpp
bool	checkEmptyName(char *src);
void	initNickname(User &user, int new_socket);
void	initName(User &user, int &new_socket);
void	connection(int fd, std::string password);


#endif
