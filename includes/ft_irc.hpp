/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:00:47 by cparras           #+#    #+#             */
/*   Updated: 2023/10/06 13:32:54 by babreton         ###   ########.fr       */
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
#include <map>
#include <vector>

#include "Channel.hpp"
#include "User.hpp"
#include "Server.hpp"

#define BUFFSIZE 1024


#endif