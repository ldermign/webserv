#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <map>
#include <iterator>
#include <fstream>
#include <vector>
#include <sys/stat.h>

#define	END_RES_LINE "\r\n"

#include "Request.hpp"
#include "Server.hpp"

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


		Response(Request *request, Server server)
			: source(request->get_source()), method(request->get_type()), server(server), location(this->match_location()), version(request->get_version()), server_name("Webserv"), 
			connection(request->get_connection())
		{
			this->set_index(this->find_index());
			this->set_status(this->create_status(request));
			this->set_date(this->get_request_date());
			this->set_content_type(this->find_content_type());
			this->set_error_name(this->init_error_name());
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
		
		void		set_response(std::string response)
		{
			this->response = response;
		}

		void		set_content_type(std::string content_type)
		{
			this->content_type = content_type;
		}
		
		void		set_date(std::string date)
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

		void		set_source(std::string source)
		{
			this->source = source;
		}

		void		set_method(std::string method)
		{
			this->method = method;
		}

		void		set_version(std::string version)
		{
			this->version = version;
		}

		void		set_body(std::string body)
		{
			this->body = body;
		}

		void		set_error_name(std::string error_name)
		{
			this->error_name = error_name;
		}

		void		set_connection(bool connection)
		{
			this->connection = connection;
		}

		void		set_server(Server server)
		{
			this->server = server;
		}

		void		set_location(std::pair<bool, Location> location)
		{
			this->location = location;
		}

		void		set_server_name(std::string server_name)
		{
			this->server_name = server_name;
		}

		void		set_path_source(std::string path_source)
		{
			this->path_source = path_source;
		}

		void		set_index(std::pair<bool, std::string> index)
		{
			this->index = index;
		}

		// statics

		static std::vector<std::string>		week_days;
		static std::vector<std::string>		months;
		static std::vector<std::string>		default_methods;

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
				return (".txt");
			return (this->get_path_source().substr(pos));
		}

		std::string		find_content_type(void)
		{
			std::string		ext;

			ext = get_ext();
			if (ext == ".html")
				return ("text/html");
			else if (ext == ".css")
				return ("text/css");
			else if (ext == ".js")
				return ("text/javascript");
			else if (ext == ".txt")
			{
				std::cout << "TEXT PLAIN" << std::endl;
				return ("text/plain");
			}
			else if (ext == ".gif")
				return ("image/gif");
			else if (ext == ".jpeg")
				return ("image/jpeg");
			else if (ext == ".png")
				return ("image/png");
			else if (ext == ".svg")
				return ("image/svg+xml");
			return ("text/html");
		}

		size_t			find_content_length(void)
		{
			return (this->body.length());
		}
		
		std::string		init_error_name(void) const
		{
			switch (this->status)
			{
				case 200:
					return ("200 OK");
				case 301:
					return ("301 Moved Permanently");
				case 400:
					return ("400 Bad Request");
				case 401:
					return ("401 Unauthorized");
				case 403:
					return ("403 Forbidden");
				case 404:
					return ("404 Not Found");
				case 405:
					return ("405 Method Not Allowed");
				case 500:
					return ("500 Internal Server Error");
				case 502:
					return ("502 Bad Gateway");
				case 503:
					return ("503 Service Unavailable");
				case 504:
					return ("504 Gateway Timeout");
				case 505:
					return ("505 HTTP Version Not Supported");
				default :
					return ("Error: Unknown error code");
			}
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

		std::string		create_autoindex(void)
		{
				std::stringstream		ss;

				this->set_content_type("text/html");

				ss << "<html>" << END_RES_LINE;
				ss << "<head><title>" << "Index of" << this->get_source()
					<< "</title></head>" << END_RES_LINE;
				ss << "<body>" << END_RES_LINE;
				ss << "<center><h1>" << this->get_source()
					<< "</h1></center>" << END_RES_LINE;
				ss << "</body>" << END_RES_LINE;
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

			if (this->status == 200)
			{
				std::ifstream		ifs(this->get_path_source().c_str());
				std::string			line;
				while (std::getline(ifs, line))
				{
					body.append(line + '\n');
				}
			}
			else if (this->status == 404 && this->is_dir(this->location.second.getRoot())
					&& this->location.second.getAutoindex())
				body = create_autoindex();
			else
			{
				body = this->create_error_response_code();
			}
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
			if (!check_version())
				return (505);
			else if (!this->is_method_allowed())
				return (405);
			else if (!request->get_format() || this->location.first == false)
				return (400);
			else if (!this->index.first)
				return (404);
			return (200);
		}

		bool	is_dir(std::string const & path) const
		{
			struct stat buffer;

			return (stat(path.c_str(), &buffer) == 0 && buffer.st_mode & S_IFDIR);
		}

		std::pair<bool, std::string>		find_index(void)
		{
			std::string		path_to_check;
			std::ifstream	ifs;
			std::vector<std::string>		&indexes = this->location.second.getIndex();
			std::pair<bool, std::string>	index;

			path_to_check.append(this->location.second.getRoot());
			path_to_check.append(this->source.substr(this->location.second.getPath().length()));
			if (*path_to_check.end() - 1 != '/')
				path_to_check += '/';
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
			return (index);
		}

		std::string		create_response(void)
		{
			std::stringstream		ss;
			std::string				response;

			ss << this->version << " " << this->get_error_name() << END_RES_LINE;
			ss << "Server: " << this->get_server_name() << END_RES_LINE;
			ss << "Date: " << this->get_date() << END_RES_LINE;
			ss << "Content-Type: " << this->get_content_type() << "\r\n";
			ss << "Content-Length: " << this->get_content_length() << "\r\n";
			ss << "Connection: " << this->get_connection_value() << "\r\n";
			ss << "\r\n";
			ss << this->get_body();
			response = ss.str();
			return (response);
		}
};
