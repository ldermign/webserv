#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <map>
#include <iterator>
#include <fstream>
#include <vector>

#define	END_RES_LINE "\r\n"

#include "Request.hpp"

class Response
{
	public :

		Response() : server("Webserv") {}

		virtual ~Response() {}

		Response&	operator=(Response const & rhs)
		{
			this->set_version(rhs.get_version());
			this->set_status(rhs.get_status());
			this->set_date(rhs.get_date());
			this->set_content_type(rhs.get_content_type());
			this->set_content_length(rhs.get_content_length());
			this->set_index(rhs.get_index());
			
			return (*this);
		}

		Response(Response const & src) : server("Webserv")
		{
			this->set_version(src.get_version());
			this->set_status(src.get_status());
			this->set_date(src.get_date());
			this->set_content_type(src.get_content_type());
			this->set_content_length(src.get_content_length());
			this->set_index(src.get_index());
		}


		Response(Request *request)
			: server("Webserv"), connection(false), index(request->get_index()), version(request->get_version())
		{
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
		
		std::string get_response(void) const
		{
			return (this->response);
		}

		std::string get_server(void) const
		{
			return (this->server);
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

		std::string		get_index(void) const
		{
			return (this->index);
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

		void		set_index(std::string index)
		{
			this->index = index;
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


	protected :

		// header data
		std::string				response;
		int 					status;
		const std::string		server;
		std::string				date;
		std::string				content_type;
//		std::string				keep_alive;
		bool					connection;
//		std::string				age;
//		std::string				x_cache_info;
		size_t					content_length;
		std::string				body;
		std::string				error_name;

		// request data
		
		std::string		index;
		std::string		version;

	private : 

		std::string	get_week_day(int week_day) const
		{
			std::map<int, std::string>		week_days;

			week_days[1] = "Mon";
			week_days[2] = "Tue";
			week_days[3] = "Wed";
			week_days[4] = "Thu";
			week_days[5] = "Fri";
			week_days[6] = "Sat";
			week_days[7] = "Sun";
			return (week_days[week_day]);
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

			pos = this->index.rfind(".");
			if (pos == std::string::npos)
				return (".txt");
			return (this->index.substr(pos));
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

		std::string		create_error_response_code(void) const
		{
			std::stringstream		ss;

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

		std::string		create_body(void) const
		{
			std::string			body;

			if (this->status == 200)
			{
				std::ifstream		ifs("./files_config/www/index.html");
				std::string			line;

				if (!ifs.is_open())
					std::cout << "file not opened" << std::endl;
				while (std::getline(ifs, line))
				{
					body.append(line + '\n');
				}
			}
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

		bool	check_version(void) const
		{
			if (!this->get_version().compare("HTTP/1.1"))
				return (true);
			return (false);
		}

		int		create_status(Request * request)
		{
			std::vector<std::string>		indexes;
			std::string						root;

			indexes.push_back("index.html");
			indexes.push_back("index.php");
			if (!request->get_format())
				return (400);
			if (!index_exist(request->get_index(), indexes))
				return (404);
			if (!check_version())
				return (505);
			return (200);
		}

		bool	index_exist(const std::string source, std::vector<std::string> indexes)
		{
			std::ifstream		ifs;

			for (std::vector<std::string>::iterator it = indexes.begin(); it != indexes.end(); ++it)
			{
				if (source == *it)
				{
					ifs.open("./files_config/www/index.html");
					if (!ifs.is_open())
						return (false);
					return (true);
				}
			}
			return (false);
		}

		std::string		create_response(void)
		{
			std::stringstream		ss;
			std::string				response;

			ss << this->version << " " << this->get_error_name() << END_RES_LINE;
			ss << "Server: " << this->get_server() << END_RES_LINE;
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
