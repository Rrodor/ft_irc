/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cparras <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:15:56 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/04 16:55:15 by cparras          ###   ########.fr       */
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
		bool				_hasNickname;
	public:
		User(int fd);
		~User();
		std::string getName();
		std::string getNickname();
		bool		getHasNickname();
		int			getFd();
		void		setName(std::string name);
		void		setNickname(std::string nickname);
		void		setFd(int fd);
		void		setHasNickname(bool hasNickname);
};

User	init(int new_socket, const char *password);