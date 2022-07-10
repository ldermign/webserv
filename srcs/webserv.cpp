#include "FtServer.hpp"
#include "webserv.hpp"

int		main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	std::string	name("cali");
	in_addr_t	domain = inet_addr("127.0.0.1");
	u_short		port = htons(4321);
	FtServer	serv(name, domain, port);
	serv.main_loop();
	return 0;
}
