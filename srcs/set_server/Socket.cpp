#include "Server.hpp"
#include "Socket.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "limits.h"

/*
 * explication : 
 * 				-> cette class recoit et envoie des requetes
 * 				-> permet de faire le lien entre le server et les fonctions de traitemetn de requetes
*/

Socket::Socket(void) : _fd(-1), _data(), _message(""), _flag(NONE), _still_connected(false), _data_server(Server()), _client_max(0)
{
	std::cout << "empty socket was created" << std::endl;
}
Socket::Socket(int fd, struct sockaddr data, int flag, Server &data_server) :	_fd(fd),
																				_data(data),
																				_message(""),
																				_flag(flag),
																				_still_connected(false),
																				_data_server(data_server),
																				_client_max(data_server.getClient())
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
	std::cout << "send!" << std::endl;
	_message = "";
	if (ret_func == -1 || ret_func == 0)
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

Response			Socket::create_response(Request const & request)
{
	Response		response(request, _data_server);

	this->set_message(response.get_response());

	return (response);
}

Request				Socket::create_request(std::string & message)
{
	Request				request(message);

	return (request);
}

bool				got_a_body(std::string str)
{
	str = str.substr(0, str.size() - 1);
	return	(str.find("\r\n\r\n") != std::string::npos) ? true : false;
}

bool				is_the_end(std::string &s1)
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
	return false or got_a_body(s1);
}

void				Socket::_receive_body(Response re, size_t &nbytes_content_length)
{
	std::vector<char>	buff_body(nbytes_content_length);
	int					ret_func;
	std::runtime_error	exp("Socket::receive_messsage()");
	Request				request;

	(void)re;
	ret_func = recv(_fd, &buff_body[0], buff_body.size(), 0);
	if (ret_func < 0)
		throw exp;
	_message.append(buff_body.begin(), buff_body.begin() + ret_func);
	nbytes_content_length -= (size_t)ret_func;
	std::cout << RED << "nbytes => " << nbytes_content_length << RESET << std::endl;
	return ;
}

void				Socket::recv_message(bool recv_accept)
{
	int					ret_func = INT_MAX;
	std::vector<char>	buff(BUFF_SIZE);
	static size_t		nbytes = 0;
	Request				request;
	Response			response;
	std::runtime_error	exp("Socket::receive_messsage()");

	if (recv_accept && nbytes == 0)
	{
		if ((ret_func = recv(_fd, &buff[0], buff.size(), 0)) < 0)
			throw exp;
		_message.append(buff.begin(), buff.begin() + ret_func);
		if (ret_func == BUFF_SIZE)
			return ;
		request = this->create_request(_message);
		if (request.get_content_length() > request.get_body().size())
			nbytes = request.get_content_length() - request.get_body().size();
		return ;
	}
	if (recv_accept && nbytes)
	{
		_receive_body(response, nbytes);
		if (nbytes == 0)
			recv_message(false);
		return ;
	}
	if (not recv_accept && _message != "")
	{
		std::cout << "RECV from " << _fd << " : " << YELLOW << _message << std::endl << RESET;
		request = this->create_request(_message);
		std::cout << "size of req -> " << request.get_body().size() << std::endl;
		response = this->create_response(request);
		_still_connected = (_still_connected) ? true : response.get_header().get_connection();
		_flag = SEND;
		return ;
	}
}

void				Socket::receive_message(void)
{
	int					ret_func = INT_MAX;
	std::vector<char>	buff(BUFF_SIZE);
	std::string			s1 = "";
	bool				first_time = true;
	std::runtime_error	exp("Socket::receive_messsage()");
	static size_t		nbytes_content_length = 0;
	bool			is_in_body_fill = false;
	Response		response;
	Request				request;
	
	if (nbytes_content_length)
	{
		_receive_body(response, nbytes_content_length);
		if (nbytes_content_length != 0)
			return ;
		is_in_body_fill = true;
	}
	else if ((ret_func = recv(_fd, &buff[0], buff.size(), 0)) > 0)
	{
		s1.append(buff.begin(), buff.begin() + ret_func);
		first_time = false;
		if (ret_func == BUFF_SIZE)
		{
			_message.append(buff.begin(), buff.begin() + ret_func);
			std::cout << "message -> \n" << _message << std::endl;
			return ;
		}
	}
	if (ret_func == -1 || (ret_func == 0 && first_time))
		throw exp;
	if (is_the_end(s1) || is_in_body_fill)
	{
		if (not is_in_body_fill)
			_message.append(buff.begin(), buff.begin() + ret_func);
		first_time = true;
		//std::cout << "Req: " << _message << std::endl;
		request = this->create_request(_message);
		if ((nbytes_content_length = request.get_content_length()) != 0 && !got_a_body(_message))
		{
			std::cout << "nbyte = " << nbytes_content_length << std::endl;
			return ;
		}
		nbytes_content_length = 0;
		response = this->create_response(request);
		_still_connected = (_still_connected) ? true : response.get_header().get_connection();
		_flag = SEND;
		return ;
	}
	_message.append(buff.begin(), buff.begin() + ret_func);
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
