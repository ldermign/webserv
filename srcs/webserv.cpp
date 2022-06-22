#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "request.hpp"
#include "webserv.hpp"

#define QUEUE_SIZE 100
#define BUFFER_SIZE 10


int 	main (int argc, char **argv)
{
	(void)argc;(void)argv;
	initServ();
	return (0);
}
