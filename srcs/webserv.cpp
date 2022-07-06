#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>
#include "request.hpp"
#include "webserv.hpp"
#include "ConfigurationFile.hpp"
#include "Server.hpp"
#include "Chaipa.hpp"

#define QUEUE_SIZE 100
#define BUFFER_SIZE 10


int 	main (int argc, char **argv)
{
	(void)argc;(void)argv;

	Chaipa allConfig;
	// if (argc != 2) {
	// 	std::cout << "Wrong number of arguments." << std::endl;
	// 	EXIT (EXIT_FAILURE);
	// }
	
	allConfig.checkAll(argv[1]);
	
	// recup_config_file(argv[1], allServ.config);
	// initServ();
	return (0);
}
