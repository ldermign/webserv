#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <map>

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


		Response(int status, std::string index, std::string version)
			: status(status), server("Webserv"), index(index), version(version)		{
			this->set_date(get_request_date());
			this->set_content_type(find_content_type());
			this->set_content_length(find_content_length());
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

	protected :

		// header data

		std::string				response;
		int 					status;
		const std::string		server;
		std::string				date;
		std::string				content_type;
//		std::string				keep_alive;
//		std::string				connection;
//		std::string				age;
//		std::string				x_cache_info;
		size_t					content_length;
//		std::string				body;

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

		std::string		find_content_type(void)
		{
			return ("TEXT/HTML");
		}

		size_t			find_content_length(void)
		{
			return (11022002);
		}

		std::string		create_response(void)
		{
			std::stringstream		ss;
			std::string				response;

			ss << this->version << " " << this->status << std::endl;
			ss << "Server: " << this->get_server() << std::endl;
			ss << "Date: " << this->get_date() << std::endl;
			ss << "Content-Type: " << this->get_content_type() << std::endl;
			ss << "Content-Length: " << this->get_content_length() << std::endl;
			ss << "Connection: " << "close" << std::endl;
			response = ss.str();
			return (response);
		}
};
