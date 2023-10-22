/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:15:56 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/16 18:47:27 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

class Channel;
class Server;

class User
{
	public:
		User();
		User(int & fd, Channel & channel, Server & server, int & fdsId);
		~User();
		User & operator=(User const & rhs);

		std::string 	getName() const;
		std::string 	getNickname() const;
		bool			getHasNickname() const;
		int				getFd() const;
		Channel			getChannel() const;
		int				getFdsId() const;

		void			setName(std::string name);
		void			setNickname(std::string nickname);
		void			setFd(int fd);
		void			setHasNickname(bool hasNickname);
		void			setChannel(Channel &channel);
		void			sendMessage(std::string message, int fd);

		void			initName();
		void			initNickname();
		void			deleteUser();
		
	private:
		int					_fd;
		int					_fdsId;
		std::string 		_name;
		std::string 		_nickname;
		bool				_hasNickname;
		Channel				&_channel;
		Server				&_server;
};

User	init(int new_socket, const char *password);

#endif
