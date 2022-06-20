#include "webserv.hpp"

int	initServ(void)
{
	
	int								socket_fd;
	int								new_socket_fd;
	socklen_t						addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in				option_bind = {	.sin_family = AF_INET,
													.sin_port = htons(PORT),
													.sin_addr = {.s_addr = INADDR_ANY}};

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(socket_fd, (const struct sockaddr*)&option_bind, sizeof(option_bind)) == -1)
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
	if ((new_socket_fd = accept(socket_fd, (struct sockaddr*)&option_bind, &addrlen)) < 0)
	{
		std::cerr << "Error while the socket accepted informations" << std::endl;
		std::cerr << "Please relaunch" << std::endl;
		return (1);
	}
	close(new_socket_fd);
	close(socket_fd);
	return (0);
}
