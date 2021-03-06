#include "Request.hpp"
#include "webserv.hpp"
#include "Parse.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include "Config.hpp"
#include "FtServer.hpp"
#include "ManagerServer.hpp"

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
		std::cout << "error code for path [" << it->getErrorPath() << "] =" << std::endl;
		{
			std::vector< int >::iterator	i;
			i = it->getErrorCode().begin();
			std::cout << "\t";
			while (i < it->getErrorCode().end()) {
				std::cout << *i;
				i++;
				if (i != it->getErrorCode().end())
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
				std::cout << "index =" << std::endl;
				{
					std::vector< std::string >::iterator	j;
					j = i->getIndex().begin();
					std::cout << "\t";
					while (j < i->getIndex().end()) {
						std::cout << *j;
						j++;
						if (j != i->getIndex().end())
							std::cout << " - ";
						else
							std::cout << std::endl;
					}
				}
				std::cout << "root = [" << i->getRoot() << "]" << std::endl;
				std::cout << "autoindex = [" << i->getAutoindex() << "]" << std::endl;
				std::cout << "methods =" << std::endl;
				{
					std::vector< std::string >::iterator	j;
					j = i->getMethods().begin();
					std::cout << "\t";
					while (j < i->getMethods().end()) {
						std::cout << *j;
						j++;
						if (j != i->getMethods().end())
							std::cout << " - ";
						else
							std::cout << std::endl;
					}
				}
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
	if (argc != 2) {
		std::cout << "Wrong number of arguments." << std::endl;
		exit (EXIT_FAILURE);
	}

	Config allConfig;
	allConfig.checkAll(argv[1]);
	std::vector< Server >	allServ = allConfig.getVectorServers();
	printAllServer(allServ);
	ManagerServer	ms(allServ);
	ms.main_loop();
	return 0;
}
