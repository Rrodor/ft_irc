/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:15:56 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/03 19:38:26 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class User
{
	private:
		int					_fd;
		std::string 		_name;
		std::string 		_nickname;
	public:
		User(int fd);
		~User();
		std::string getName();
		std::string getNickname();
		int			getFd();
		void		setName(std::string name);
		void		setNickname(std::string nickname);
		void		setFd(int fd);
};
