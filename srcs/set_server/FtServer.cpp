#include "FtServer.hpp"
/*
 * explication :
 * 					-> initialise le server (avec un domain, port, et name) par des fonctions (socket(), bind(), listen())
 * 					-> ensuite gere des sockets par des fonctions (select()) qui generent des objets de class Socket qui recv() et send() des requetes
*/


FtServer::FtServer(void) : _main_socket_fd(-1), _port(htons(1234)), _domain(INADDR_ANY), _name ("Jose"), _set({0})
{}

FtServer::FtServer(std::string &name, in_addr_t &domain, u_short &port) : _main_socket_fd(-1), _port(port), _domain(domain), _name (name) _set({0})
{}

~FtServer(void)
{}

const FtServer& FtServer::operator=(const FtServer& fs)
{
	_domain = fs._domain;
	_port = fs._port;
	_name = fs._name;
	_main_socket_fd = fs._main_socket_fd;
	_set[0] = fs._set[0];
	_set[1] = fs._set[1];
	_set[2] = fs._set[2];
}


int			FtServer::_init_server(void)
{
	
	::_create_main_socket();
	::_bind_main_socket();
	return 0;
}

const int			&FtServer::get_main_fd(void) const
{
	return _main_socket_fd;
}

Socket		&FtServer::get_last_client(void)
{
	return (_queue_fd.pop());
}

void					FtServer::_create_main_socket(void)
{
	std::runtime_error		socket_error("socket()");

	_main_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_main_socket_fd == -1)
		throw socket_error;
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
}

void					FtServer::_bind_main_socket(void)
{
	struct sockaddr_in				option_bind = {	.sin_family = AF_INET,
													.sin_port = _port,
													.sin_addr = {.s_addr = _domain}};
	int								yes = 1;
	int								res;
	std::runtime_error				bind_error("bind()");
	std::runtime_error				setsockopt_error("setsockopt()");
	std::runtime_error				listen_error("listen()");

	res = setsockopt(_main_socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if (res == -1)
		throw setsockopt_error;
	res = bind(_main_socket_fd, (const struct sockaddr*)&option_bind, sizeof(option_bind));
	if (res == -1)
		throw bind_error;
	res = listen(_main_socket_fd, 128);
	if (res == -1)
		throw listen_error;
	Socket		new_one(_main_socket_fd, option_bind, ACCEPT);
	_fds.push_back(new_one);
}

void					FtServer::_select_socket(void)
{
	int					ret_select;
	int					h_fd = 0;
	struct timeval		tv = {.tv_sec = 0, .tv_usec = 5000};
	std::runtime_error	select_fail("select()");

	for (int i = 0; i < 3; i++)
	{
		FD_ZERO(_set[i]);
		for (std::vector<Socket>::iterator it = _fds.begin(); it != _fds.end(); it++)
			FD_SET(it->get_fd());
	}
	for (std::vector<Socket>::iterator it = _fds.begin(); it != _fds.end(); it++)
		h_fd = (it->get_fd() > h_fd) ? it->get_fd() : h_fd;
	ret_select = select(h_fd + 1, _set, _set + 1, _set + 2, &tv);
	if (ret_select == -1)
		throw select_fail;
}

void					FtServer::_action_socket(void)
{
	for (std::vector<Socket>::iterator it = _fds.begin(); it != _fds.end; it++)
	{
		if (it->get_flag() == ACCEPT && FD_ISSET(it->get_fd(), &_set[0]))
			_fds.push_back(it->accept_new_socket());
		else if (it->get_flag == RECV && FD_ISSET(it->get_fd(), &_set[0]))
		{
			try
				it->receive_message();
			catch (std::exception &e)
			{
				std::cout << "Error -> " << e.what() << std:endl;
				it->destroy();
				_fds.erase(it--);
			}
		}
		else if (it->get_flag == SEND && FD_ISSET(it->get_fd(), &_set[1]))
		{
			it->send_message();
			it->destroy();
			_fds.erase(it--);
		}
	}
}

int					FtServer::main_loop(void)
{
	try 
	{
		::_init_server();
		std::cout << "server_initialize" << std::endl;
		while (true)
		{
			::_select_socket();
			::_action_socket();
		}
	}
	catch (std::exception &e)
	{
		std::cout << "Error -> " << e.what() << std::endl;
		return 1;
	}
}
