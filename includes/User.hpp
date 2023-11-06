/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 17:48:02 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/04 18:44:15 by rrodor           ###   ########.fr       */
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
};

#endif
