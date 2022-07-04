#pragma once
#include <queue>
#include <iostream>
#include <arpa/inet.h>

class FtClient
{
	protected :
		int						_fd;
		struct sockaddr_storage	_data;
	public :
		FtClient(void);
		FtClient(int &fd, struct sockaddr_storage &data);
		FtClient& operator=(FtClient& fc);
		~FtClient(void);
		std::string	&get_message(void);
		void		send_message(std::string &message);
		void		destroy(void);
};
