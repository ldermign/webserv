#include "webserv.hpp"
#include "request.hpp"

int		recv_request(int fd)
{
	char 	buff[BUFFER_SIZE];

	memset(buff, 0, BUFFER_SIZE);
	if (recv(fd, buff, BUFFER_SIZE, 0) < 1)
			return (0);
	execute_request(buff);
	return (0);
}

int	initServ(void)
{
	int								socket_fd;
	int								new_socket_fd = -2;
	socklen_t						addrlen = sizeof(struct sockaddr_in);

	struct sockaddr_in				serv_bind = {	.sin_family = AF_INET,
													.sin_port = htons(PORT),
													.sin_addr = {.s_addr = INADDR_ANY},
													.sin_zero = {0}};
	struct pollfd					fds[1] = {{0}};

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(socket_fd, (const struct sockaddr*)&serv_bind, sizeof(serv_bind)) == -1)
	{
		std::cerr << "Error while binding the server" << std::endl;
		std::cerr << "Please relaunch" << std::endl;
		return (1);
	}
	if (listen(socket_fd, QUEUE_SIZE) == -1)
	{
		std::cerr << "Error while listen the server" << std::endl;
		std::cerr << "Please relaunch" << std::endl;
		return (1);
	}
	for ( ; ; )
	{
		if ((new_socket_fd = accept(socket_fd, (struct sockaddr*)&cli_bind, &addrlen)) < 0)
		{
			std::cerr << "Error while the socket accepted informations" << std::endl;
			std::cerr << "Please relaunch" << std::endl;
			return (1);
		}
		recv_request(new_socket_fd);
		if (send(new_socket_fd, "HTTP/1.0 200 KO\r\n\r\nHello world", 24, 0) == -1)
			std::cout << "Error while sending" << std::endl;
		close(new_socket_fd);
	}
	close(socket_fd);
	return (0);
}
