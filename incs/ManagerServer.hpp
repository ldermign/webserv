#pragma once
#include <iostream>
#include <vector>
#include "webserv.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "FtServer.hpp"

class ManagerServer
{
	private :
		std::vector<Server>		_all_data;
		std::vector<FtServer>		_all_servers;
		void	_init_servers(void)
		{
			for (std::vector<Server>::iterator it = _all_data.begin(); it < _all_data.end(); it++)
			{
				 std::string	name(*(it->getServerName().begin()));
				//std::cout << "this new serv details : name -> " << name << ", host -> " << it->getHost() << ", port -> " << it->getPort() << std::endl;
				 in_addr_t	domain = inet_addr(it->getHost().c_str());
				 u_short		port = htons(it->getPort());
				 try {
				 	FtServer	serv(name, domain, port, *it);
				 	_all_servers.push_back(serv);
				 } catch (std::exception &e)
				 {
					 std::cout << "error occured while initialization of " << name << ": " << e.what() << std::endl;
					 continue ;
				 }
			}
		}
	public :
		void	main_loop(void)
		{
			if (_all_servers.size() < 1)
				return ;
			while (true)
			{
				for (std::vector<FtServer>::iterator it = _all_servers.begin(); it < _all_servers.end(); it++)
					it->main_loop();	
			}
		}
	ManagerServer(std::vector<Server> &all_data) : _all_data(all_data)
	{
		_init_servers();
	}
	ManagerServer(void) : _all_data()
	{
		std::cout << "nothing will append with this manager of server" << std::endl;
	}
	~ManagerServer(void)
	{}
	ManagerServer&	operator=(const ManagerServer &ma)
	{
		*this = ma;
		return *this;
	}
};
