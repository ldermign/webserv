#include "FtServer.hpp"
/*
 * explication :
 * 					-> initialise le server (avec un domain, port, et name) par des fonctions (socket(), bind(), listen())
 * 					-> ensuite gere des sockets par des fonctions (select()) qui generent des objets de class Socket qui recv() et send() des requetes
*/


FtServer::FtServer(void) : _main_socket_fd(-1), _port(htons(1234)), _domain(INADDR_ANY), _name ("Jose"), _set()
{}

FtServer::FtServer(std::string &name, in_addr_t &domain, u_short &port) : _main_socket_fd(-1), _port(port), _domain(domain), _name (name), _set()
{
}

FtServer::~FtServer(void)
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
	return *this;
}


void		FtServer::_init_server(void)
{
	
	std::cout << RED << "main_fd in creation .." << RESET << "\r" << std::flush;
	_create_main_socket();
	std::cout << RED << "main_fd in creation ..." << RESET << "\r" << std::flush;
	_bind_main_socket();
	std::cout << RMLINE << GREEN << "main_fd created ! (" << _main_socket_fd << ")"<< RESET << std::endl << std::endl;
}

const int			&FtServer::get_main_fd(void) const
{
	return _main_socket_fd;
}

void					FtServer::_create_main_socket(void)
{
	std::runtime_error		socket_error("socket()");

	_main_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_main_socket_fd == -1)
		throw socket_error;
	fcntl(_main_socket_fd, F_SETFL, O_NONBLOCK);
}

void					FtServer::_bind_main_socket(void)
{
	struct sockaddr_in				option_bind = {	.sin_family = AF_INET,
													.sin_port = _port,
													.sin_addr = {.s_addr = _domain},
													.sin_zero = {0}};
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
	Socket		new_one(_main_socket_fd, *((struct sockaddr*)&option_bind), ACCEPT);
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
		FD_ZERO(_set + i);
		for (std::vector<Socket>::iterator it = _fds.begin(); it != _fds.end(); it++)
		{
			FD_SET(it->get_fd(), _set + i);
		}
	}
	for (std::vector<Socket>::iterator it = _fds.begin(); it != _fds.end(); it++)
		h_fd = (it->get_fd() > h_fd) ? it->get_fd() : h_fd;
	ret_select = select(h_fd + 1, _set, _set + 1, _set + 2, &tv);
	if (ret_select == -1)
		throw select_fail;
}

void					FtServer::_action_socket(void)
{
	std::vector<Socket> buff_add;
	std::vector<Socket> buff_rem;

	for (std::vector<Socket>::iterator it = _fds.begin(); it != _fds.end(); it++)
	{
		if (it->get_flag() == ACCEPT && FD_ISSET(it->get_fd(), &_set[0]))
		{
			Socket	fd = it->accept_new_socket();
			std::cout << "new ACCEPT : " << GREEN << fd.get_fd() << RESET << std::endl;
			buff_add.push_back(fd);
		}
		else if (it->get_flag() == RECV && FD_ISSET(it->get_fd(), &_set[0]))
		{
			try
			{
				it->receive_message();
				if (it->get_flag() != RECV)
					std::cout << "RECV from "<< it->get_fd() <<" : \n" << YELLOW << it->get_message()<< RESET << std::endl;
			}
			catch (std::exception &e)
			{
				std::cout << "Error -> " << e.what() << std::endl;
				Socket	fd = *it;
				buff_rem.push_back(fd);
				it->destroy();
			}
		}
		else if (it->get_flag() == SEND && FD_ISSET(it->get_fd(), &_set[0]))
		{
			it->send_message();
			Socket	fd = *it;
			buff_rem.push_back(fd);
			std::cout << "SEND to "<< it->get_fd() <<" : \n" << BLUE << it->get_message() << RESET << std::endl;
			it->destroy();
		}
	}
	for (std::vector<Socket>::iterator it2 = buff_rem.begin(); it2 != buff_rem.end(); it2++)
	{		
		for (std::vector<Socket>::iterator it = _fds.begin(); it != _fds.end(); it++)
		{
			if (it2->get_fd() == it->get_fd())
			{
				_fds.erase(it);
				break ;
			}
		}
	}
	_fds.insert(_fds.end(), buff_add.begin(), buff_add.end());
}

int					FtServer::main_loop(void)
{
	try 
	{
		_init_server();
		while (true)
		{
			_select_socket();
			_action_socket();
		}
	}
	catch (std::exception &e)
	{
		std::cout << "Error -> " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
