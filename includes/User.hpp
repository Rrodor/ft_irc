/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 17:48:02 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/16 15:35:57 by babreton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _USER_HPP_
#define _USER_HPP_

#include "ft_irc.hpp"

class User
{
	public:
		int			fd;
		std::string	username;
		std::string	nickname;
		std::string	realname;

		User(int & fd);
		~User();

		bool	operator==(User & user);
		bool	operator!=(User & user);
};

#endif
