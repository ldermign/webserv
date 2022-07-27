#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <iterator>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>

#define	END_RES_LINE "\r\n"

#include "Request.hpp"
#include "Server.hpp"
#include "FileData.hpp"


class Response
{
	public :

		Response() : server_name("Webserv") {}

		virtual ~Response() {}

		Response&	operator=(Response const & rhs)
		{
			this->set_server(rhs.get_server());
			this->set_location(rhs.get_location());
			this->set_response(rhs.get_response());
			this->set_index(rhs.get_index());

			// first line response

			this->set_status(rhs.get_status());
			this->set_version(rhs.get_version());

			// header reponse

			this->set_date(rhs.get_date());
			this->set_content_type(rhs.get_content_type());
			this->set_content_length(rhs.get_content_length());
			this->set_connection(rhs.get_connection());

			// body response

			this->set_body(rhs.get_body());
			this->set_error_name(rhs.get_error_name());

			this->set_source(rhs.get_source());
			this->set_method(rhs.get_method());
	
			return (*this);
		}

		Response(Response const & src)
		{
			this->set_server(src.get_server());
			this->set_location(src.get_location());
			this->set_response(src.get_response());
			this->set_index(src.get_index());

			// first line response
			this->set_status(src.get_status());
			this->set_version(src.get_version());

			// header reponse

			this->set_date(src.get_date());
			this->set_content_type(src.get_content_type());
			this->set_content_length(src.get_content_length());
			this->set_connection(src.get_connection());
			this->set_server_name(src.get_server_name());

			// body response

			this->set_body(src.get_body());

			this->set_source(src.get_source());
			this->set_method(src.get_method());
		}


		Response(Request *request, Server & server)
			: source(request->get_source()), method(request->get_type()), server(server), location(this->match_location()), version(request->get_version()), server_name("Webserv"), 
			connection(request->get_connection())
		{
			this->set_index(this->find_index());
			this->set_status(this->create_status(request));
			this->set_date(this->get_request_date());
			this->set_content_type(this->find_content_type());
			this->set_error_name(this->find_status_message());
			this->set_body(this->create_body());
			this->set_content_length(this->find_content_length());
			this->set_response(this->create_response());
		}

		void	print_response(void)
		{
			std::cout << this->get_response();
		}

		// Getters
		
		Server		get_server(void) const
		{
			return (this->server);
		}
		
		std::string get_response(void) const
		{
			return (this->response);
		}

		std::string get_server_name(void) const
		{
			return (this->server_name);
		}

		std::string get_content_type(void) const
		{
			return (this->content_type);
		}
		
		std::string get_date(void) const
		{
			return (this->date);
		}

		size_t	 get_content_length(void) const
		{
			return (this->content_length);
		}

		int		get_status(void) const
		{
			return (this->status);
		}

		std::string		get_version(void) const
		{
			return (this->version);
		}

		std::string		get_source(void) const
		{
			return (this->source);
		}

		std::string		get_method(void) const
		{
			return (this->method);
		}

		std::string		get_body(void) const
		{
			return (this->body);
		}

		std::string		get_error_name(void) const
		{
			return (this->error_name);
		}

		bool			get_connection(void) const
		{
			return (this->connection);
		}

		std::pair<bool, Location>	get_location(void) const
		{
			return (this->location);
		}

		std::string		get_path_source(void) const
		{
			return (this->path_source);
		}
		
		std::pair<bool, std::string>	get_index(void) const
		{
			return (this->index);
		}

		// Setters
		
		void		set_response(std::string const & response)
		{
			this->response = response;
		}

		void		set_content_type(std::string const & content_type)
		{
			this->content_type = content_type;
		}
		
		void		set_date(std::string const & date)
		{
			this->date = date;
		}

		void		set_content_length(size_t content_length)
		{
			this->content_length = content_length;
		}

		void		set_status(int status)
		{
			this->status = status;
		}

		void		set_source(std::string const & source)
		{
			this->source = source;
		}

		void		set_method(std::string const & method)
		{
			this->method = method;
		}

		void		set_version(std::string const & version)
		{
			this->version = version;
		}

		void		set_body(std::string const & body)
		{
			this->body = body;
		}

		void		set_error_name(std::string const & error_name)
		{
			this->error_name = error_name;
		}

		void		set_connection(bool connection)
		{
			this->connection = connection;
		}

		void		set_server(Server const & server)
		{
			this->server = server;
		}

		void		set_location(std::pair<bool, Location> const & location)
		{
			this->location = location;
		}

		void		set_server_name(std::string const & server_name)
		{
			this->server_name = server_name;
		}

		void		set_path_source(std::string const & path_source)
		{
			this->path_source = path_source;
		}

		void		set_index(std::pair<bool, std::string> const & index)
		{
			this->index = index;
		}

		// statics

		static std::vector<std::string>		week_days;
		static std::vector<std::string>		months;
		static std::vector<std::string>		default_methods;
		static std::vector<int>				redirection_status;
		static std::map<int, std::string>	status_messages;

	protected :

		// request data
		
		std::string		source;
		std::string		method;

		// response data

		std::string								response;
		Server									server;
		std::pair<bool, Location>				location;
		std::string								path_source;
		std::pair<bool, std::string>			index;


		// first line response

		int 					status;
		std::string				version;

		// header data

		std::string				server_name;
		std::string				date;
		std::string				content_type;
		size_t					content_length;
		bool					connection;

		// body response

		std::string				body;
		std::string				error_name;


	private : 

		static std::vector<std::string>		init_week_days(void)
		{
			std::vector<std::string>		days;

			days.push_back("Sun");
			days.push_back("Mon");
			days.push_back("Tue");
			days.push_back("Wed");
			days.push_back("Thu");
			days.push_back("Fri");
			days.push_back("Sat");
			return (days);
		}

		static std::vector<std::string>		init_months(void)
		{
			std::vector<std::string>		months;

			months.push_back("Jan");
			months.push_back("Feb");
			months.push_back("Mar");
			months.push_back("Apr");
			months.push_back("May");
			months.push_back("Jun");
			months.push_back("Jul");
			months.push_back("Aug");
			months.push_back("Sep");
			months.push_back("Oct");
			months.push_back("Nov");
			months.push_back("Dec");
			return (months);
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

		std::string	get_week_day(int week_day) const
		{
			return (this->week_days[week_day]);
		}

		std::string	get_day(int day) const
		{
			std::stringstream		ss;

			if (day < 10)
				ss << 0;
			ss << day;
			return (ss.str());
		}
		
		std::string	get_month(int month) const
		{
			std::map<int, std::string>		months;

			months[0] = "Jan";
			months[1] = "Feb";
			months[2] = "Mar";
			months[3] = "Apr";
			months[4] = "May";
			months[5] = "Jun";
			months[6] = "Jul";
			months[7] = "Aug";
			months[8] = "Sep";
			months[9] = "Oct";
			months[10] = "Nov";
			months[11] = "Dec";
			return (months[month]);
		}

		int			get_year(int year) const
		{
			return (year + 1900);
		}

		std::string	get_request_date(void) const
		{
			std::stringstream		date;
			time_t					time;
			tm						*gmt;

			time = std::time(NULL);
			gmt = std::gmtime(&time);

			date << this->get_week_day(gmt->tm_wday);
			date << ", " ;
			date << this->get_day(gmt->tm_mday);
			date << " ";
			date << this->get_month(gmt->tm_mon);
			date << " ";
			date << this->get_year(gmt->tm_year);
			date << " ";
			date << gmt->tm_hour;
			date << ":";
			date << gmt->tm_min;
			date << ":";
			date << gmt->tm_sec;
			date << " ";
			date << "GMT";
			return (date.str());
		}

		std::string		get_ext(void)
		{
			size_t			pos;

			pos = this->get_path_source().find(".");
			if (pos == std::string::npos)
				return ("");
			return (this->get_path_source().substr(pos));
		}

		std::string		find_content_type(void)
		{
			std::string		ext;

			ext = get_ext();
			if (this->location.second.getReturnCode()
					&& !this->is_redirection(this->location.second.getReturnCode()))
				return ("application/octet-stream");
			else if (ext == ".html")
				return ("text/html");
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
			return ("text/plain");
		}

		size_t			find_content_length(void)
		{
			return (this->body.length());
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

		std::string		create_error_response_code(void)
		{
			std::stringstream		ss;

			this->set_content_type("text/html");
			ss << "<html>" << END_RES_LINE;
			ss << "<head><title>" << this->get_error_name()
				<< "</title></head>" << END_RES_LINE;
			ss << "<body>" << END_RES_LINE;
			ss << "<center><h1>" << this->get_error_name()
				<< "</h1></center>" << END_RES_LINE;
			ss << "<hr><center>Webserv/1.0 (42)</center>" << END_RES_LINE;
			ss << "</body>" << END_RES_LINE;
			ss << "</html>" << END_RES_LINE;
			return (ss.str());
		}

		std::map<std::string, FileData>			get_content_dir(std::string const & dir_name) const
		{
			std::map<std::string, FileData>		content_dir;
			DIR						*dir;
			struct dirent			*diread;

			dir = opendir(dir_name.c_str());
			if (dir)
			{
				while ((diread = readdir(dir)))
				{
					FileData					file(dir_name, diread->d_name);

					content_dir[diread->d_name] = file;
				}
			}
			return (content_dir);
		}

		std::string		add_spaces(unsigned int n, size_t offset)
		{
			std::string		spaces;

			if (n < offset)
				offset = n - 1;
			spaces.append(n - offset, ' ');

			return (spaces);
		}

		std::string		index_in_autoindex(std::string const & index, const size_t space_gap, bool dir)
		{
			std::string			trimmed_index;

			if (index.length() >= space_gap)
			{
				trimmed_index = index.substr(0, space_gap - 4);
				if (dir)
					trimmed_index.append("../");
				else
					trimmed_index.append("...");
				return (trimmed_index);
			}
			return (index);
		}

		std::string		link_autoindex(std::string const & dest) const
		{
			std::string			link;

			link = this->source;
			if (*(link.end() - 1) != '/')
				link.append(1, '/');
			link.append(dest);
			std::cout << "LOCATION PATH = " << location.second.getPath() << std::endl;
			std::cout << "LINK = " << link  << std::endl;
			return (link);
		}

		std::string		create_autoindex(void)
		{
				std::stringstream						ss;
				std::map<std::string, FileData>			content;
				const size_t							first_space_gap = 40;
				const size_t							second_space_gap = 60;

				content = get_content_dir(this->get_path_source());
				this->set_content_type("text/html");
				ss << "<html>" << END_RES_LINE;
				ss << "<head><title>Index of " << this->get_source() << "</title></head>" << END_RES_LINE;
				ss << "<body>" << END_RES_LINE;
				ss << "<h1>Index of " << this->get_source() << "</h1><hr><pre>" << END_RES_LINE;
				for (std::map<std::string, FileData>::iterator it = content.begin(); it != content.end(); ++it)
				{
					if (!this->is_dir(it->second.get_path()))
						continue ;
					ss << "<a href=" << this->link_autoindex(it->first) << ">";
					ss << this->index_in_autoindex(it->first, first_space_gap, true);
					if (it->first.length() < first_space_gap)
						ss << "/";
					ss << "</a>";
					ss << this->add_spaces(first_space_gap, it->second.get_name().length() + 1);
					ss << it->second.get_edit_date();
					ss << this->add_spaces(second_space_gap, it->second.get_edit_date().length());
					ss << "-" << END_RES_LINE;
				}

				for (std::map<std::string, FileData>::iterator it = content.begin(); it != content.end(); ++it)
				{
					if (this->is_dir(it->second.get_path()))
						continue ;
					ss << "<a href=" << this->link_autoindex(it->first) << ">";
					ss << this->index_in_autoindex(it->first, first_space_gap, false) << "</a>";
					ss << this->add_spaces(first_space_gap, it->second.get_name().length());
					ss << it->second.get_edit_date();
					ss << this->add_spaces(second_space_gap, it->second.get_edit_date().length());
					ss << it->second.get_size() << END_RES_LINE;
				}
				ss << "</pre><hr></body>" << END_RES_LINE;
				ss << "</html>" << END_RES_LINE;
				return (ss.str());
		}

		bool			root_exist(void) const
		{
			struct stat buffer;

			return (stat(this->location.second.getRoot().c_str(), &buffer) == 0);
		}

		std::string		create_body(void)
		{
			std::string			body;

			if (this->location.second.getReturnCode()
					&& !this->is_redirection(this->location.second.getReturnCode()))
				body = this->location.second.getReturnPath();
			else if ((this->status == 200 && !this->location.second.getAutoindex())
					|| (this->location.second.getAutoindex() && this->index.first))
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
				body = create_autoindex();
			}
			else
				body = this->create_error_response_code();
			return (body);
		}

		std::string		get_connection_value(void)
		{
			if (this->connection)
				return ("keep-alive");
			else
				return ("close");
		}

		bool	check_version(void)
		{
			if (this->get_version().empty())
			{
				this->set_version("HTTP/1.1");
				return (true);
			}
			else if (!this->get_version().compare("HTTP/1.1"))
				return (true);
			this->set_version("HTTP/1.1");
			return (false);
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

				if (!this->get_source().compare(0, it->getPath().length(), it->getPath()))
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
						this->method) != this->default_methods.end());
		}

		bool	is_method_allowed(void)
		{
			std::vector<std::string>		&methods_allowed = this->location.second.getMethods();

			if (!methods_allowed.size())
				return (std::find(this->default_methods.begin(),
						this->default_methods.end(), this->method)
						!= this->default_methods.end());
			return (std::find(methods_allowed.begin(), methods_allowed.end(),
						this->method) != methods_allowed.end());
		}

		int		create_status(Request * request)
		{
			std::cout << "PATH = " << this->get_path_source();
			if (!check_version())
				return (505);
			else if (!this->is_method_allowed())
				return (405);
			else if (!request->get_format() || this->location.first == false)
				return (400);
			else if (this->location.second.getReturnCode())
				return (this->location.second.getReturnCode());
			else if (!this->index.first && (!this->location.second.getAutoindex() ||
						(this->location.second.getAutoindex() && !this->is_dir(this->get_path_source()))))
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
			location_path = this->source.substr(this->location.second.getPath().length());

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

		std::pair<bool, std::string>		find_index(void)
		{
			std::string		path_to_check;
			std::ifstream	ifs;
			std::vector<std::string>		&indexes = this->location.second.getIndex();
			std::pair<bool, std::string>	index;

			path_to_check = this->create_path_to_index();
			if (this->is_file(path_to_check))
			{
				index.first = true;
				this->set_path_source(path_to_check);
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
							index.first = true;
							index.second = *it;
							this->set_path_source(path_to_check);
							ifs.close();
							return (index);
						}
					}
					ifs.close();
					path_to_check.erase(path_to_check.length() - it->length(), it->length());
				}
				index.first = false;
				this->set_path_source(path_to_check);
			}
			return (index);
		}

		bool			is_redirection(unsigned int status)
		{
			return (std::find(this->redirection_status.begin(),
						this->redirection_status.end(),
						status) != this->redirection_status.end());
		}

		std::string		create_response(void)
		{
			std::stringstream		ss;
			std::string				response;

			ss << this->version << " " << this->get_error_name() << END_RES_LINE;
			ss << "Server: " << this->get_server_name() << END_RES_LINE;
			ss << "Date: " << this->get_date() << END_RES_LINE;
			ss << "Content-Type: " << this->get_content_type() << END_RES_LINE;
			ss << "Content-Length: " << this->get_content_length() << END_RES_LINE;
			if (this->is_redirection(this->status) || this->get_status() == 300)
				ss << "Location: http://" << this->server.getHost() \
					<< ":" << this->server.getPort() << this->location.second.getReturnPath() << END_RES_LINE;
			ss << "Connection: " << this->get_connection_value() << END_RES_LINE;
			ss << END_RES_LINE;
			ss << this->get_body();
			response = ss.str();
			return (response);
		}
};
