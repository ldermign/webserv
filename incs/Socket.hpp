#pragma once
#include "webserv.hpp"
#include <iostream>
#include <arpa/inet.h>

class Socket
{
	protected :
		int						_fd;
		struct sockaddr			_data;
		std::string				_message;
		int						_flag;
	public :
		Socket(void);
		Socket(int fd, struct sockaddr data, int flag);
		Socket& operator=(const Socket& fc);
		~Socket(void);
		const std::string	&get_message(void) const;
		int					get_fd(void) const;
		struct sockaddr		get_data(void) const;
		int					get_flag(void) const;
		void				set_flag(int flag);
		void				set_message(std::string new_message);
		Socket				accept_new_socket(void);
		void				send_message(void);
		void				create_response(std::string & message);
		void				receive_message(void);
		void				destroy(void);
};
