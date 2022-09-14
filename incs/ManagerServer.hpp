#pragma once
#include <iostream>
#include <vector>
#include "webserv.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "FtServer.hpp"

bool g_keep_going = true;

void handle_death (int num)
{
	(void)num;
	g_keep_going = false;
}

std::string check_host(std::string s1)
{
	std::cout << "s1 -> " << s1 << std::endl;
	if(s1 != "127.0.0.1" && s1 != "localhost")
		throw (std::runtime_error("bind()"));
	return "127.0.0.1";
}

class ManagerServer
{
	private :
		std::vector<Server>		_all_data;
		std::vector<FtServer>		_all_servers;
		void	_init_servers(void)
		{
			for (std::vector<Server>::iterator it = _all_data.begin(); it < _all_data.end(); it++)
			{
				std::string name;
				if (it->getServerName().begin() != it->getServerName().end())
					name = *(it->getServerName().begin());
				else
					name = "default";
				in_addr_t	domain = inet_addr(check_host(it->getHost()).c_str());
				u_short		port = htons(it->getPort());
				FtServer	serv(name, domain, port, *it);
				_all_servers.push_back(serv);
			}
		}
	public :
		void	main_loop(void)
		{
			if (_all_servers.size() < 1)
				return ;
			signal(SIGINT, handle_death);
			while (g_keep_going)
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
	{
		for (std::vector<FtServer>::iterator ite = _all_servers.begin(); ite < _all_servers.end(); ite++)
		{
			ite->destroy_me();
		}
	}
	ManagerServer&	operator=(const ManagerServer &ma)
	{
		(void)ma;
		return *this;
	}
};
