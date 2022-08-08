#pragma once

#define	END_RES_LINE "\r\n"

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <iterator>
#include <vector>
#include <cstdio>

#include <sys/stat.h>

#include "Request.hpp"
#include "Server.hpp"
#include "ResponseHeader.hpp"
#include "ResponseBody.hpp"

class Response
{
	public :

		Response() {}

		virtual ~Response() {}

		Response&	operator=(Response const & rhs)
		{
			this->set_server(rhs.get_server());
			this->set_location(rhs.get_location());
			this->set_response(rhs.get_response());
			this->set_index(rhs.get_index());
			this->set_autoindex(rhs.get_autoindex());

			// first line response

			this->set_status(rhs.get_status());
			this->set_version(rhs.get_version());

			// header reponse

			this->set_header(rhs.get_header());

			// body response

			this->set_body(rhs.get_body());
			this->set_status_message(rhs.get_status_message());

			// request data

			this->set_request(rhs.get_request());
	
			return (*this);
		}

		Response(Response const & rhs)
		{
			this->set_server(rhs.get_server());
			this->set_location(rhs.get_location());
			this->set_response(rhs.get_response());
			this->set_index(rhs.get_index());
			this->set_autoindex(rhs.get_autoindex());

			// first line response

			this->set_status(rhs.get_status());
			this->set_version(rhs.get_version());

			// header

			this->set_header(rhs.get_header());

			// body

			this->set_body(rhs.get_body());


			// request

			this->set_request(rhs.get_request());
		}


		Response(Request const & request, Server const & server)
			: request(request), server(server), location(this->match_location()), version("HTTP/1.1")
		{
			this->set_header(ResponseHeader("Webserv", request.get_connection(),
					this->get_server(), this->get_location()));
			this->set_index(this->find_index());
			this->set_status(this->create_status(request));
			this->header.set_status(this->get_status());
			this->header.set_content_type(this->find_content_type());
			this->process_method();
			this->set_status_message(this->find_status_message());
			this->set_body(ResponseBody(this->request, this->get_status(),
			this->get_index(), this->get_index_path(), this->get_status_message(),
			this->get_server(), this->get_location()));
			this->header.set_content_length(this->find_content_length());
			this->set_response(this->create_response());
		}

		void	print_response(void)
		{
			std::cout << this->get_response();
		}

		// Getters

		Request		get_request(void) const
		{
			return (this->request);
		}

		Server		get_server(void) const
		{
			return (this->server);
		}
		
		std::string get_response(void) const
		{
			return (this->response);
		}

		int		get_status(void) const
		{
			return (this->status);
		}

		std::string		get_version(void) const
		{
			return (this->version);
		}

		ResponseBody	get_body(void) const
		{
			return (this->body);
		}

		std::string		get_status_message(void) const
		{
			return (this->status_message);
		}

		ResponseHeader		get_header(void) const
		{
			return (this->header);
		}

		std::pair<bool, Location>	get_location(void) const
		{
			return (this->location);
		}

		std::string		get_index_path(void) const
		{
			return (this->index_path);
		}
		
		bool		get_index(void) const
		{
			return (this->index);
		}

		Autoindex						get_autoindex(void) const
		{
			return (this->autoindex);
		}

		// Setters

		void		set_request(Request const & request)
		{
			this->request = request;
		}
	
		void		set_response(std::string const & response)
		{
			this->response = response;
		}

		void		set_status(int status)
		{
			this->status = status;
		}

		void		set_version(std::string const & version)
		{
			this->version = version;
		}

		void		set_body(ResponseBody const & body)
		{
			this->body = body;
		}

		void		set_status_message(std::string const & status_message)
		{
			this->status_message = status_message;
		}


		void		set_server(Server const & server)
		{
			this->server = server;
		}

		void		set_location(std::pair<bool, Location> const & location)
		{
			this->location = location;
		}

		void		set_index_path(std::string const & index_path)
		{
			this->index_path = index_path;
		}

		void		set_index(bool index)
		{
			this->index = index;
		}

		void		set_autoindex(Autoindex const & autoindex)
		{
			this->autoindex = autoindex;
		}

		void		set_header(ResponseHeader const & header)
		{
			this->header = header;
		}

		// statics

		static std::vector<std::string>		default_methods;
		static std::map<int, std::string>	status_messages;
		static std::vector<int>				redirection_status;

	protected :

		// request data

		Request			request;

		// response data

		std::string								response;
		Server									server;
		std::pair<bool, Location>				location;
		std::string								index_path;
		bool									index;
		Autoindex								autoindex;

		// first line response

		int 					status;
		std::string				version;

		// header data

		ResponseHeader			header;

		// body response

		ResponseBody			body;
		std::string				status_message;


	private : 

		void	process_method(void)
		{
			if (!this->get_request().get_method().compare("DELETE"))
			{
				if (this->get_status() == 200)
				{
					if (remove(this->get_index_path().c_str()) != 0)
					{
						this->set_status(403);
						this->header.set_status(403);
					}
				}
				// delete resssource
			}
			else if (!this->get_request().get_method().compare("GET"))
			{
				// parse params
			}
			else if (!this->get_request().get_method().compare("POST"))
			{
				// parse params
			}
		}

		static std::vector<std::string>		init_default_methods(void)
		{
			std::vector<std::string>		default_methods;

			default_methods.push_back("GET");
			default_methods.push_back("POST");
			default_methods.push_back("DELETE");
			return (default_methods);
		}

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

		static std::map<int, std::string>		init_status_messages(void)
		{
			std::map<int, std::string>		status_messages;

			status_messages[200] = "200 OK";
			status_messages[301] = "301 Moved Permanently";
			status_messages[302] = "302 Found";
			status_messages[303] = "303 See Other";
			status_messages[307] = "307 Temporary Redirect";
			status_messages[308] = "308 Permanent Redirect";
			status_messages[400] = "400 Bad request";
			status_messages[401] = "401 Unauthorized";
			status_messages[403] = "403 Forbidden";
			status_messages[404] = "404 Not Found";
			status_messages[405] = "405 Method Not Allowed";
			status_messages[500] = "500 Internal Server status";
			status_messages[502] = "502 Bad Gateway";
			status_messages[503] = "503 Service Unavailable";
			status_messages[504] = "504 Gateway Timeout";
			status_messages[505] = "505 HTTP Version Not Supported";
			return (status_messages);
		}

		std::string		get_ext(std::string const & file)
		{
			size_t			pos;

			pos = file.rfind(".");
			if (pos == std::string::npos)
				return ("");
			return (this->get_index_path().substr(pos));
		}

		std::string		find_content_type(void)
		{
			std::string		ext;

			ext = get_ext(this->get_index_path());
			if (this->location.second.getReturnCode()
					&& !this->is_redirection(this->location.second.getReturnCode()))
				return ("application/octet-stream");
			else if (this->get_status() != 200 || !this->get_request().get_method().compare("DELETE")
					|| (this->get_status() == 200 && this->is_dir(this->location.second.getRoot())
						&& this->location.second.getAutoindex() &&
						this->is_dir(this->get_index_path())) || !ext.compare(".html")
							|| !ext.compare(".php"))
			{
				return ("text/html");
			}
			else if (ext == ".css")
				return ("text/css");
			else if (ext == ".js")
				return ("text/javascript");
			else if (ext == ".txt")
				return ("text/plain");
			else if (ext == ".gif")
				return ("image/gif");
			else if (ext == ".jpeg")
				return ("image/jpeg");
			else if (ext == ".png")
				return ("image/png");
			else if (ext == ".svg")
				return ("image/svg+xml");
			return ("application/octet-stream");
		}

		size_t			find_content_length(void)
		{
			return (this->body.get_body().length());
		}
		
		std::string		find_status_message(void) const
		{
			std::stringstream		ss;
			std::string				status_message;

			if (this->status_messages[this->get_status()].empty())
			{
				ss << this->get_status();
				status_message = ss.str();
				while (status_message.length() < 3)
					status_message.insert(status_message.begin(), '0');
				return (status_message);
			}
			return (this->status_messages[this->get_status()]);
		}

		bool			root_exist(void) const
		{
			struct stat buffer;

			return (stat(this->location.second.getRoot().c_str(), &buffer) == 0);
		}

		bool			is_redirection(unsigned int status)
		{
			return (std::find(this->redirection_status.begin(),
						this->redirection_status.end(),
						status) != this->redirection_status.end());
		}

		bool	check_version(void)
		{
			return (!this->request.get_version().compare("HTTP/1.1")
				|| this->request.get_version().empty());
		}

		std::pair<bool, Location>		match_location(void)
		{
			std::vector<Location>						locations;
			std::pair<bool, Location>					location;
			std::vector<Location>::const_iterator		longest_match;
			
			locations = this->server.getLocation();
			location.first = false;
			longest_match = locations.end();
			for (std::vector<Location>::const_iterator it = locations.begin();
					it != locations.end(); ++it)
			{
				if (!this->get_request().get_source().compare(0, it->getPath().length(), it->getPath()))
				{
					if (longest_match == locations.end() ||
						it->getPath().length() > longest_match->getPath().length())
					{
						location.first = true;
						location.second = *it;
						longest_match = it;
					}
				}
			}
			return (location);
		}

		bool	is_default_method(void) const
		{
			return (std::find(this->default_methods.begin(), this->default_methods.end(),
						this->get_request().get_method()) != this->default_methods.end());
		}

		bool	is_method_allowed(void)
		{
			std::vector<std::string>		&methods_allowed = this->location.second.getMethods();

			if (!methods_allowed.size())
				return (std::find(this->default_methods.begin(),
						this->default_methods.end(), this->get_request().get_method())
						!= this->default_methods.end());
			return (std::find(methods_allowed.begin(), methods_allowed.end(),
						this->get_request().get_method()) != methods_allowed.end());
		}

		int		create_status(Request const & request)
		{
			if (!request.get_format() || this->location.first == false)
				return (400);
			else if (!check_version())
				return (505);
			else if (!this->is_method_allowed())
				return (405);
			else if (this->location.second.getReturnCode())
				return (this->location.second.getReturnCode());
			else if (!this->index && (!this->location.second.getAutoindex() ||
						(this->location.second.getAutoindex() &&
						 (!this->is_dir(this->get_index_path())
						  || !this->get_request().get_method().compare("DELETE")))))
				return (404);
			return (200);
		}

		bool	is_dir(std::string const & path) const
		{
			struct stat buffer;

			return (stat(path.c_str(), &buffer) == 0 && buffer.st_mode & S_IFDIR);
		}

		bool	is_file(std::string const & path) const
		{
			struct stat buffer;

			return (stat(path.c_str(), &buffer) == 0 && buffer.st_mode & S_IFREG);
		}

		std::string							create_path_to_index(void) const
		{
			std::string				path_to_index;
			std::string				root;
			std::string				location_path;

			root = this->location.second.getRoot();
			location_path = this->get_request().get_source().substr(this->location.second.getPath().length());
			if (*location_path.begin() == '/' && *(root.end() - 1) == '/')
				root.erase(root.end() - 1);
			if (*location_path.begin() != '/' && *(root.end() - 1) != '/')
				location_path.insert(location_path.begin(), '/');
			path_to_index.append(root);
			path_to_index.append(location_path);
			if (!this->is_file(path_to_index))
			{
				if (*(path_to_index.end() - 1) != '/')
					path_to_index += '/';
			}
			return (path_to_index);
		}

		bool		 		find_index(void)
		{
			std::string						path_to_check;
			std::ifstream					ifs;
			std::vector<std::string>		&indexes = this->location.second.getIndex();
			bool							index;

			path_to_check = this->create_path_to_index();
			if (this->is_file(path_to_check))
			{
				index = true;
				this->set_index_path(path_to_check);
			}
			else
			{
				for (std::vector<std::string>::const_iterator it = indexes.begin(); it != indexes.end(); ++it)
				{
					path_to_check.append(*it);
					ifs.open(path_to_check.c_str());
					if (ifs.is_open())
					{
						if (!this->is_dir(path_to_check))
						{
							index = true;
							this->set_index_path(path_to_check);
							ifs.close();
							return (index);
						}
					}
					ifs.close();
					path_to_check.erase(path_to_check.length() - it->length(), it->length());
				}
				index = false;
				this->set_index_path(path_to_check);
			}
			return (index);
		}

		std::string		create_response(void)
		{
			std::stringstream		ss;
			std::string				response;

			ss << this->version << " " << this->get_status_message() << END_RES_LINE;
			ss << this->get_header().create_header();
			ss << END_RES_LINE;
			ss << this->get_body().create_body();
			response = ss.str();
			return (response);
		}
};
