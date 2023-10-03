/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:15:56 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/03 16:23:11 by rrodor           ###   ########.fr       */
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
};
