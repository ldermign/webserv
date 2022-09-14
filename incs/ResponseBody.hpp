#pragma once

#include <string>
#include <fstream>

#include "Autoindex.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Request.hpp" 
#include "Cgi.hpp"

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
			this->set_request(rhs.get_request());
			this->set_location(rhs.get_location());
			this->set_status(rhs.get_status());
			this->set_index(rhs.get_index());
			this->set_autoindex(rhs.get_autoindex());
			this->set_index_path(rhs.get_index_path());
			this->set_status_message(rhs.get_status_message());
			this->set_content_length(rhs.get_content_length());
		}

		virtual ~ ResponseBody()
		{

		}

		ResponseBody&		operator=(ResponseBody const & rhs)
		{
			this->set_body(rhs.get_body());
			this->set_server(rhs.get_server());
			this->set_request(rhs.get_request());
			this->set_location(rhs.get_location());
			this->set_status(rhs.get_status());
			this->set_index(rhs.get_index());
			this->set_autoindex(rhs.get_autoindex());
			this->set_index_path(rhs.get_index_path());
			this->set_status_message(rhs.get_status_message());
			this->set_content_length(rhs.get_content_length());
			return (*this);
		}

		ResponseBody(Request const & request, int status, bool index,
				std::string const & index_path, std::string const & status_message,
				Server const & server, std::pair<bool, Location> const & location)
				: server(server), request(request), location(location),
				status(status), index(index), index_path(index_path),
				status_message(status_message)
		{
			this->set_autoindex(Autoindex(this->get_request(), this->get_index_path()));
			this->set_body(this->create_body());
		}

		std::string		get_ext(std::string const & file)
		{
			size_t			pos;

			pos = file.rfind(".");
			if (pos == std::string::npos)
				return ("");
			return (this->get_index_path().substr(pos));
		}

		std::string		execute_cgi(std::string const & script_path)
		{
			Cgi				cgi(script_path, this->get_request(), this->get_location().second);

			return (cgi.exec_script());
		}

		bool	check_cgi(void)
		{
			if (this->get_index() && !this->get_ext(this->get_index_path()).compare(".php"))
			{
				return (!this->get_location().second.getCgi().first.compare(".php")
						&& this->is_dir(this->get_location().second.getCgi().second));
			}
			return (true);
		}

		std::string		create_body(void)
		{
			std::string			body;

			if (!this->get_request().get_method().compare("DELETE") && this->get_status() == 200)
				body = this->delete_success();
			else if (this->location.second.getReturnCode()
					&& !this->is_redirection(this->location.second.getReturnCode()))
				body = this->location.second.getReturnPath();
			else if ((this->get_status() == 200 && !this->location.second.getAutoindex())
					|| ((this->get_status() == 200 || (this->get_status() == 404 && !check_cgi())) &&
						this->location.second.getAutoindex() && this->index &&
						!this->is_redirection(this->location.second.getReturnCode()))
					|| (this->get_status() == 200 && check_cgi() && !this->get_ext(this->get_index_path()).compare(".php")))
			{
				std::ifstream		ifs(this->get_index_path().c_str());
				std::string			line;

				while (std::getline(ifs, line))
				{
					body.append(line + '\n');
				}
				ifs.close();
				if (!this->get_ext(this->get_index_path()).compare(".php"))
					body = execute_cgi(this->get_index_path());
			}
			else if (this->status == 200 && this->is_dir(this->location.second.getRoot())
					&& this->location.second.getAutoindex())
			{
				body = this->autoindex.create_autoindex();
			}
			else
			{
				body = this->create_error_response();
			}
			this->set_content_length(body.length());
			return (body);
		}

		// setters

		void		set_body(std::string const & body)
		{
			this->body = body;
		}

		void		set_request(Request const & request)
		{
			this->request = request;
		}

		void		set_server(Server const & server)
		{
			this->server = server;
		}

		void		set_location(std::pair<bool, Location> const & location)
		{
			this->location = location;
		}

		void		set_status(int status)
		{
			this->status = status;
		}

		void		set_index(bool index)
		{
			this->index = index;
		}

		void		set_autoindex(Autoindex const & autoindex)
		{
			this->autoindex = autoindex;
		}

		void		set_index_path(std::string const & index_path)
		{
			this->index_path = index_path;
		}

		void		set_status_message(std::string const & status_message)
		{
			this->status_message = status_message;
		}
		
		void		set_content_length(size_t content_length)
		{
			this->content_length = content_length;
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

		int		get_status(void) const
		{
			return (this->status);
		}

		bool 	get_index(void) const
		{
			return (this->index);
		}

		Autoindex						get_autoindex(void) const
		{
			return (this->autoindex);
		}

		std::string		get_index_path(void) const
		{
			return (this->index_path);
		}

		std::string		get_status_message(void) const
		{
			return (this->status_message);
		}

		Request			get_request(void) const
		{
			return (this->request);
		}

		size_t			get_content_length(void) const
		{
			return (this->content_length);
		}

		// static

		static std::vector<int>				redirection_status;

	private :

		std::string						body;
		Server							server; 
		Request							request;
		std::pair<bool, Location>		location;
		int								status;
		bool							index;
		Autoindex						autoindex;
		std::string						index_path;
		std::string						status_message;
		size_t							content_length;

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

		std::string		create_default_error_response(void)
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

		std::pair<int, std::string> find_error_personalized(void)
		{
			std::vector<std::pair<int, std::string> >		error_page = this->server.getErrorPage();
			std::pair<int, std::string>						not_found;

			for (std::vector<std::pair<int, std::string> >::const_iterator it = error_page.begin();
					it != error_page.end(); ++it)
			{
				if (it->first == this->get_status())
					return (*it);
			}
			not_found.first = -1;
			return (not_found);
		}

		std::string		create_error_response(void)
		{
			std::string						error_response;
			std::pair<int, std::string>		error_page;

			error_page = find_error_personalized();
			if (error_page.first != -1)
			{
				std::ifstream		ifs(error_page.second.c_str());
				std::string			line;

				while (std::getline(ifs, line))
				{
					error_response.append(line + '\n');
				}
			}
			else
				error_response = create_default_error_response();
			return (error_response);
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
