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
	u_short		port2 = htons(4322);
	u_short		port3 = htons(4324);
	FtServer	serv(name, domain, port);
	FtServer	serv2(name, domain, port2);
	FtServer	serv3(name, domain, port3);
	while (true)
	{
		serv.main_loop();
		serv2.main_loop();
		serv3.main_loop();
	}

	
	//allConfig.checkAll(argv[1]);

	return 0;
}
