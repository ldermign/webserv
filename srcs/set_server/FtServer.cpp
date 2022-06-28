#include "FtServer"


FtServer::FtServer(void) : _main_socket_fd(-1), _port(htons(1234)), _domain(INADDR_ANY), _name ("Jose")
{}

FtServer::FtServer(std::string &name, in_addr_t &domain, u_short &port) : _main_socket_fd(-1), _port(port), _domain(domain), _name (name)
{}

~FtServer(void)
{}

const FtServer& FtServer::operator=(const FtServer& fs)
{
	_domain = fs._domain;
	_port = fs._port;
	_name = fs._name;
	_main_socket_fd = fs._main_socket_fd;
}

int			FtServer::init_server(void) throw()
{
	try 
	{
		::_create_main_fd();
		::_bind_main_socket();
		::_fill_queue();
	}
	catch (std::exception &e)
	{
		std::err << "ERROR : " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

const int			&FtServer::get_main_fd(void) const
{
	return _main_socket_fd;
}

FtClient		&FtServer::get_last_client(void)
{
	return (_queue_fd.pop());
}

void					FtServer::_create_main_socket(void);
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
}
void					FtServer::main_loop(void)
{
	struct sockaddr_storage their_addr;
	bool					is_listen = false;
	0

	res = listen(_main_socket_fd, 128);
	if (res == -1)
		throw listen_error;
	
}
