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
{ (void)argc;(void)argv;
	return (0);
}

