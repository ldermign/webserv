#pragma once
#include "webserv.hpp"
#include <queue>
#include <iostream>
#include <arpa/inet.h>

class Socket
{
	protected :
		int						_fd;
		struct sockaddr_storage	_data;
		std::string				_message;
	public :
		Socket(void);
		Socket(int &fd, struct sockaddr_storage &data);
		Socket& operator=(const Socket& fc);
		~Socket(void);
		const std::string	&get_message(void) const;
		void				send_message(void) const;
		void				receive_message(void);
		void				destroy(void);
};
