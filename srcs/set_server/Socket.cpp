#include "Socket.hpp"
#include "Communication.hpp"

/*
 * explication : 
 * 				-> cette class recoit et envoie des requetes
 * 				-> permet de faire le lien entre le server et les fonctions de traitemetn de requetes
*/
Socket::Socket(void) : _fd(-1), _data(), _message(""), _flag(NONE), _still_connected(false)
{
	std::cout << "empty socket was created" << std::endl;
}
Socket::Socket(int fd, struct sockaddr data, int flag) : _fd(fd), _data(data), _message(""), _flag(flag), _still_connected(false)
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

	_message = _message.substr(0, _message.size() - 1);
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
	Socket				new_one(ret_func, their_addr, RECV);
	return new_one;
}

Response*			Socket::create_response(std::string	& message)
{
	Communication		communication(message);

	this->set_message(communication.get_response());
	return (communication.get_res());
}

bool				is_the_end(std::string s1)
{
	std::cout << "s1 => " << s1 << std::endl;
	size_t	var_i = s1.size();
	while ((s1.c_str()[var_i] != '\n' && s1.c_str()[var_i] != '\r') && var_i > 0)
		var_i--;
	if (var_i != 0)
		var_i--;
	while ((s1.c_str()[var_i] != '\n' && s1.c_str()[var_i] != '\r') && var_i > 0)
		var_i--;
	if (s1.c_str()[var_i] == '\n' || s1.c_str()[var_i] == '\r')
		var_i++;
	if (!std::strcmp(s1.c_str() + var_i, "\r\n") || !std::strcmp(s1.c_str() + var_i, "\n") || !std::strcmp(s1.c_str() + var_i, ""))
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
	std::string			request("GET index.html HTTP/1.0\r\nconnection: keep-alive\r\n");
	
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
		std::cout << "Request:" << std::endl;
		std::cout << this->_message << std::endl;
		this->create_response(this->_message);
		_flag = SEND;
		return ;
	}
	_message.append(buff.begin(), buff.end());
	response = this->create_response(this->_message);
	_still_connected = (_still_connected) ? true : response->get_connection();
	_flag = SEND;
	return ;
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
