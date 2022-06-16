#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define QUEUE_SIZE 100
#define PORT 3245
#define BUFFER_SIZE 10

int		recv_maybe(int fd, char *buff)
{
	while (true)
	{
		if (recv(fd, buff, BUFFER_SIZE, 0) < 1)
				return (0);
		std::cout << "received : " << buff << std::endl;
	}
	std::cout << "end of comunication." << std::endl;
	return (0);
}

int 	main (int argc, char **argv)
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
	
	//ici se passe la suite, il faut deja voir comment on va gerer les macros QUEUE_SIZE et PORT et surtout 
	//comment on les utilises
	//
	//ensuite voici un code qui est utile pour tester rudimentairement avec telnet que le code fonctionne.
	/*
	char	buff[BUFFER_SIZE];
	recv_maybe(new_socket_fd, (char*)&buff);
	*/
	// telnet :
	/*
	 * telnet localhost 3245
	 */
	close(new_socket_fd);
	close(socket_fd);
	return (0);
}

