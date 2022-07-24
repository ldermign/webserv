#include "Server.hpp"
#include "Socket.hpp"
#include "Communication.hpp"

/*
 * explication : 
 * 				-> cette class recoit et envoie des requetes
 * 				-> permet de faire le lien entre le server et les fonctions de traitemetn de requetes
*/
Socket::Socket(void) : _fd(-1), _data(), _message(""), _flag(NONE), _still_connected(false), _data_server(Server())
{
	std::cout << "empty socket was created" << std::endl;
}
Socket::Socket(int fd, struct sockaddr data, int flag, const Server &data_server) :	_fd(fd),
																					_data(data),
																					_message(""),
																					_flag(flag),
																					_still_connected(false),
																					_data_server(data_server)
{}
Socket& Socket::operator=(const Socket& fc)
{
	_fd = fc._fd;
	_data = fc._data;
	_message = fc._message;
	_flag = fc._flag;
	return *this;
}
Socket::~Socket(void)
{}

int					Socket::get_fd(void) const
{
	return _fd;
}
const std::string&	Socket::get_message(void) const
{
	return _message;
}

bool				Socket::still_connected(void) const
{
	return _still_connected;
}

void				Socket::set_message(std::string new_message)
{
	_message = new_message;
}
void				Socket::send_message(void)
{
	int					ret_func;
	std::runtime_error	exp("send()");

	ret_func = send(_fd, _message.c_str(), _message.size(), 0);
	std::cout << "SEND to "<< get_fd() <<" : \n" << BLUE << get_message() << RESET << std::endl;
	_message = "";
	if (ret_func == -1)
		throw exp;
	_flag = RECV;
}

int				Socket::get_flag(void) const
{
	return _flag;
}

void				Socket::set_flag(int flag)
{
	_flag = flag;
}

Socket				Socket::accept_new_socket(void)
{
	int					ret_func;
	struct sockaddr		their_addr;
	std::runtime_error	accept_error("accept()");
	socklen_t			s_addr = sizeof(their_addr);

	ret_func = accept(_fd, &their_addr, &s_addr);
	if (ret_func == -1)
		throw accept_error;
	Socket				new_one(ret_func, their_addr, RECV, _data_server);
	return new_one;
}

Response*			Socket::create_response(std::string	& message)
{
	std::cout << "BEFORE" << std::endl;
	Communication		communication(message, _data_server);

	std::cout << "AVANT" << std::endl;
	this->set_message(communication.get_response());
	std::cout << "APRES = " << communication.get_res()->get_response() << std::endl;

	return (communication.get_res());
}

bool				is_the_end(std::string s1)
{
	if (!std::strcmp(s1.c_str(), "\r\n") || !std::strcmp(s1.c_str(), "\n"))
		return true;
	const char	*str = s1.c_str();
	size_t		var_i = s1.size() - 1;
	while ((str[var_i] == '\n' || str[var_i] == '\r') && var_i > 0)
		var_i --;
	var_i++;
	if (!std::strncmp(str + var_i, "\n\r\n", 3) || !std::strcmp(str + var_i, "\n\n") || !std::strncmp(str + var_i, "\r\n\n", 3) || !std::strncmp(str + var_i, "\r\n\r\n", 4))
		return true;
	return false;
}

void				Socket::receive_message(void)
{
	int					ret_func;
	std::vector<char>	buff(BUFF_SIZE);
	std::string			s1 = "";
	bool				first_time = true;
	std::runtime_error	exp("Socket::receive_messsage()");
	Response			*response;
	
	std::cout << "I got to receive " << std::endl;
	if ((ret_func = recv(_fd, &buff[0], buff.size(), 0)) > 0)
	{
		s1.append(buff.begin(), buff.begin() + ret_func);
		first_time = false;
		std::cout << "size of s1 -> " << s1.size() << std::endl;
	}
	if (ret_func == -1 || (ret_func == 0 && first_time))
		throw exp;
	_message.append(buff.begin(), buff.begin() + ret_func);
	if (is_the_end(s1))
	{
		std::cout << "RECV from "<< get_fd() <<" : \n" << YELLOW << get_message()<< RESET << std::endl;
		first_time = true;
		response = this->create_response(this->_message);
		std::cout << "AFTER" << std::endl;
		_still_connected = (_still_connected) ? true : response->get_connection();
		_flag = SEND;
		return ;
	}
}

struct sockaddr		Socket::get_data(void) const
{
	return _data;
}

void				Socket::destroy(void)
{
	if (_fd > 2)
		close (_fd);
	std::cout << "End of " << RED << _fd << RESET << std::endl;
}
