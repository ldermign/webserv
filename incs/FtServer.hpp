#pragma once
#include "webserv.hpp"
#include "Socket.hpp"
#include "Server.hpp"


class FtServer
{
	protected :
		int 					_main_socket_fd;
		std::vector<Socket>		_fds;
		u_short					_port;
		in_addr_t				_domain;
		std::string				_name;
		fd_set					_set[3];
		Server					_data;
		void					_create_main_socket(void);
		void					_bind_main_socket(void);
		void					_select_socket(void);
		void					_action_socket(void);
		void					_init_server(void);

	public :

		FtServer(void);
		FtServer(std::string &name, in_addr_t &domain, u_short &port, Server &data);
		~FtServer(void);
		const FtServer& operator=(const FtServer& fs);
		const int		&get_main_fd(void) const;
		int				main_loop(void);
		void			destroy_me(void);
};
