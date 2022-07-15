#include "Socket.hpp"
#include "Communication.hpp"

/*
 * explication : 
 * 				-> cette class recoit et envoie des requetes
 * 				-> permet de faire le lien entre le server et les fonctions de traitemetn de requetes
*/
Socket::Socket(void) : _fd(-1), _data(), _message(""), _flag(NONE) 
{
	std::cout << "empty socket was created" << std::endl;
}
Socket::Socket(int fd, struct sockaddr data, int flag) : _fd(fd), _data(data), _message(""), _flag(flag)
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

void				Socket::set_message(std::string new_message)
{
	_message = new_message;
}
void				Socket::send_message(void)
{
	int					ret_func;
	std::runtime_error	exp("send()");

	ret_func = send(_fd, _message.c_str(), _message.size(), 0);
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

void				Socket::receive_message(void)
{
	int					ret_func;
	std::vector<char>	buff(BUFF_SIZE);
	bool				first_time = true;
	std::runtime_error	exp("Socket::receive_messsage()");
	Response			*response;
//	std::string			request("GET index.html HTTP/1.0\r\n");
	
	_message = "";
	while ((ret_func = recv(_fd, &buff[0], buff.size(), 0)) > 0)
	{
		_message.append(buff.begin(), buff.end());
		first_time = false;
		break ;
	}
	if (ret_func == -1 || (ret_func == 0 && first_time))
		throw exp;
	response = this->create_response(this->_message);

	// usage: 
	// response->get_connection()
	// 1) if true ==> keep-alive 
	// 2) if false ==> close

	_flag = SEND;
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
