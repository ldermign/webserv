#pragma once
#include <queue>
#include <iostream>
#include <arpa/inet.h>
#include "Socket.hpp"

class FtServer
{
	protected :
		int 					_main_socket_fd;
		std::vector<Socket>	_queue_fd;
		u_short					_port;
		in_addr_t				_domain;
		std::string				_name;
		void					_create_main_socket(void);
		void					_bind_main_socket(void);


	public :
		FtServer(void);
		FtServer(std::string &name, in_addr_t &domain, u_short &port);
		~FtServer(void);
		const FtServer& operator=(const FtServer& fs);
		void			init_server(void);
		int				get_main_fd(void);
		Socket		get_last_client(void);
		void			main_loop(void);
};
