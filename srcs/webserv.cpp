#include "Request.hpp"
#include "webserv.hpp"
#include "ConfigurationFile.hpp"
#include "Server.hpp"
#include "FtServer.hpp"
#include "Chaipa.hpp"

int		main(int argc, char **argv)
{
	(void)argc;(void)argv;

	Chaipa allConfig;

	std::string	name("cali");
	in_addr_t	domain = inet_addr("127.0.0.1");
	u_short		port = htons(4321);
	FtServer	serv(name, domain, port);
	serv.main_loop();

	
	//allConfig.checkAll(argv[1]);

	return 0;
}
