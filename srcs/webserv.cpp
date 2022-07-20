#include "Request.hpp"
#include "webserv.hpp"
#include "Parse.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include "Config.hpp"
#include "FtServer.hpp"

void	printAllServer( std::vector< Server > all ) {

	int ret = 1;
	std::vector< Server >::iterator	it;

	it = all.begin();
	while (it < all.end()) {

		std::cout << "\n\t~~~~~~~~~~\tServer num " << ret << "\t~~~~~~~~~~" << std::endl;
		std::cout << "host = [" << it->getHost() << "]" << std::endl;
		std::cout << "port = [" << it->getPort() << "]" << std::endl;
		std::cout << "server name =" << std::endl;
		{
			std::vector< std::string >::iterator	i;
			i = it->getServerName().begin();
			std::cout << "\t";
			while (i < it->getServerName().end()) {
				std::cout << *i;
				i++;
				if (i != it->getServerName().end())
					std::cout << " - ";
				else
					std::cout << std::endl;
			}
		}
		{
			int ret2 = 1;
			std::vector< Location >::iterator	i;
			i = it->getLocation().begin();
			while (i < it->getLocation().end()) {
				std::cout << std::endl << "Location " << ret2 << " ~~~~~" << std::endl;
				std::cout << "path = [" << i->getPath() << "]" << std::endl;
				std::cout << "root = [" << i->getRoot() << "]" << std::endl;
				std::cout << "autoindex = [" << i->getAutoindex() << "]" << std::endl;
				std::cout << "return code = [" << i->getReturnCode() << "]" << std::endl;
				std::cout << "return path = [" << i->getReturnPath() << "]" << std::endl;
				std::cout << "cgi extension = [" << i->getCgiExtension() << "]" << std::endl;
				std::cout << "cgi path = [" << i->getCgiPath() << "]" << std::endl;
				std::cout << "download = [" << i->getDownload() << "]" << std::endl;
				i++;
				ret2++;
			}
		}
		it++;
		ret++;
	}
	std::cout << std::endl;
}

int		main(int argc, char **argv)
{
	(void)argc;(void)argv;

	Config allConfig;
	allConfig.checkAll(argv[1]);
	printAllServer(allConfig.getVectorServers());

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

	return 0;
}
