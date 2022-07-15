#include "request.hpp"
#include "webserv.hpp"
#include "Parse.hpp"
#include "Location.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include "FtServer.hpp"

int		main(int argc, char **argv)
{
	(void)argc;(void)argv;

	Config allConfig;

	// std::string	name("cali");
	// in_addr_t	domain = inet_addr("127.0.0.1");
	// u_short		port = htons(4321);
	// FtServer	serv(name, domain, port);
	// serv.main_loop();

	
	allConfig.checkAll(argv[1]);

	return 0;
}
