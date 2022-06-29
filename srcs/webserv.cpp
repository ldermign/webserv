#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>
#include "request.hpp"
#include "webserv.hpp"

#define QUEUE_SIZE 100
#define BUFFER_SIZE 10


int 	main (int argc, char **argv)
{
	(void)argc;(void)argv;
	// if (argc != 2) {
	// 	std::cout << "Wrong number of arguments." << std::endl;
	// 	EXIT (EXIT_FAILURE);
	// }
	// recup_config_file(argv[1]);
	initServ();
	return (0);
}
