#pragma once

#include <string>
#include <fstream>

#include "Autoindex.hpp"
#include "Server.hpp"
#include "Location.hpp"

class ResponseBody
{
	public :

		ResponseBody()
		{

		}

		ResponseBody(ResponseBody const & rhs)
		{
			this->set_body(rhs.get_body());
			this->set_server(rhs.get_server());
			this->set_location(rhs.get_location());
			this->set_method(rhs.get_method());
			this->set_status(rhs.get_status());
			this->set_index(rhs.get_index());
			this->set_autoindex(rhs.get_autoindex());
			this->set_path_source(rhs.get_path_source());
			this->set_status_message(rhs.get_status_message());
		}

		virtual ~ ResponseBody()
		{

		}

		ResponseBody&		operator=(ResponseBody const & rhs)
		{
			this->set_body(rhs.get_body());
			this->set_server(rhs.get_server());
			this->set_location(rhs.get_location());
			this->set_method(rhs.get_method());
			this->set_status(rhs.get_status());
			this->set_index(rhs.get_index());
			this->set_autoindex(rhs.get_autoindex());
			this->set_path_source(rhs.get_path_source());
			this->set_status_message(rhs.get_status_message());

			return (*this);
		}

		ResponseBody(std::string const & method, std::string const & source,
				int status, std::pair<bool, std::string> index,
				std::string const & path_source, std::string const & status_message,
				Server const & server, std::pair<bool, Location> const & location) : server(server),
				location(location), method(method), source(source), status(status), index(index),
				path_source(path_source), status_message(status_message)
		{
			this->set_autoindex(Autoindex(this->get_source(), this->get_path_source()));
			this->set_body(this->create_body());
		}

		std::string		create_body(void)
		{
			std::string			body;

			if (!this->get_method().compare("DELETE") && this->get_status() == 200)
				body = this->delete_success();
			else if (this->location.second.getReturnCode()
					&& !this->is_redirection(this->location.second.getReturnCode()))
				body = this->location.second.getReturnPath();
			else if ((this->get_status() == 200 && !this->location.second.getAutoindex())
					|| (this->location.second.getAutoindex() && this->index.first
						&& !this->is_redirection(this->location.second.getReturnCode())))
			{
				std::ifstream		ifs(this->get_path_source().c_str());
				std::string			line;
				while (std::getline(ifs, line))
				{
					body.append(line + '\n');
				}
			}
			else if (this->status == 200 && this->is_dir(this->location.second.getRoot())
					&& this->location.second.getAutoindex())
			{
				body = this->autoindex.create_autoindex();
			}
			else
				body = this->create_error_response_code();
			return (body);
		}

		// setters

		void		set_body(std::string const & body)
		{
			this->body = body;
		}

		void		set_server(Server const & server)
		{
			this->server = server;
		}

		void		set_location(std::pair<bool, Location> const & location)
		{
			this->location = location;
		}

		void		set_method(std::string const & method)
		{
			this->method = method;
		}

		void		set_status(int status)
		{
			this->status = status;
		}

		void		set_source(std::string const & source)
		{
			this->source = source;
		}

		void		set_index(std::pair<bool, std::string> const & index)
		{
			this->index = index;
		}

		void		set_autoindex(Autoindex const & autoindex)
		{
			this->autoindex = autoindex;
		}

		void		set_path_source(std::string const & path_source)
		{
			this->path_source = path_source;
		}

		void		set_status_message(std::string const & status_message)
		{
			this->status_message = status_message;
		}

		// getters

		std::string		get_body(void) const
		{
			return (this->body);
		}

		Server		get_server(void) const
		{
			return (this->server);
		}

		std::pair<bool, Location>	get_location(void) const
		{
			return (this->location);
		}

		std::string		get_method(void) const
		{
			return (this->method);
		}
		
		std::string		get_source(void) const
		{
			return (this->source);
		}

		int		get_status(void) const
		{
			return (this->status);
		}

		std::pair<bool, std::string>	get_index(void) const
		{
			return (this->index);
		}

		Autoindex						get_autoindex(void) const
		{
			return (this->autoindex);
		}

		std::string		get_path_source(void) const
		{
			return (this->path_source);
		}

		std::string		get_status_message(void) const
		{
			return (this->status_message);
		}

		// static

		static std::vector<int>				redirection_status;

	private :

		std::string						body;
		Server							server; 
		std::pair<bool, Location>		location;
		std::string						method;
		std::string						source;
		int								status;
		std::pair<bool, std::string>	index;
		Autoindex						autoindex;
		std::string						path_source;
		std::string						status_message;

		static std::vector<int>		init_redirection_status(void)
		{
			std::vector<int>		redirection_status;

			redirection_status.push_back(301);
			redirection_status.push_back(302);
			redirection_status.push_back(303);
			redirection_status.push_back(304);
			redirection_status.push_back(307);
			redirection_status.push_back(308);
			return (redirection_status);
		}

		bool	is_dir(std::string const & path) const
		{
			struct stat buffer;

			return (stat(path.c_str(), &buffer) == 0 && buffer.st_mode & S_IFDIR);
		}

		bool			is_redirection(unsigned int status)
		{
			return (std::find(this->redirection_status.begin(),
						this->redirection_status.end(),
						status) != this->redirection_status.end());
		}

		std::string		create_error_response_code(void)
		{
			std::stringstream		ss;

			ss << "<html>" << END_RES_LINE;
			ss << "<head><title>" << this->get_status_message()
				<< "</title></head>" << END_RES_LINE;
			ss << "<body>" << END_RES_LINE;
			ss << "<center><h1>" << this->get_status_message()
				<< "</h1></center>" << END_RES_LINE;
			ss << "<hr><center>Webserv/1.0 (42)</center>" << END_RES_LINE;
			ss << "</body>" << END_RES_LINE;
			ss << "</html>" << END_RES_LINE;
			return (ss.str());
		}

		std::string		delete_success(void) const
		{
			std::stringstream		ss;

			ss << "<html>" << END_RES_LINE;
			ss << "<body>" << END_RES_LINE;
		    ss << "<h1>File deleted.</h1>" << END_RES_LINE;
			ss << "</body>" << END_RES_LINE;
			ss << "</html>" << END_RES_LINE;
			return (ss.str());
		}

};
