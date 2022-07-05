#include "Socket.hpp"


Socket::Socket(void) : _fd(-1), _data(), _message("")
{
	std::cout << "empty socket was created" << std::endl;
}
Socket::Socket(int &fd, struct sockaddr_storage &data) : _fd(fd), _data(data), _message("")
{}
Socket& Socket::operator=(const Socket& fc)
{
	_fd = fc._fd;
	_data = fc._data;
	_message = fc._message;
	return *this;
}
Socket::~Socket(void)
{}

const std::string&	Socket::get_message(void) const
{
	return _message;
}
void				Socket::send_message(void) const
{
	int					ret_func;
	std::runtime_error	exp("send()");

	ret_func = send(_fd, message.c_str(), _message.size(), 0);
	if (ret_func == -1)
		throw exp;

}
void				Socket::receive_message(void)
{
	int					ret_func;
	std::vector<char>	buff(BUFF_SIZE);
	bool				first_time = true;
	std::runtime_error	exp("recv()");
	
	_message = "";
	while ((ret_func = recv(_fd, &buff[0], buff.size(), 0)) > 0)
	{
		_message.append(buff.cbegin(), buff.cend());
		first_time = false;
	}
	if (ret_func == -1)
		throw exp;
	if (ret_func == 0 && first_time)
		destroy();
}
void				Socket::destroy(void)
{
	if (_fd > 2)
		close (_fd);
	delete this;
}
