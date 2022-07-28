#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include <map>
#include <vector>
#include <algorithm>

#include "Server.hpp"
#include "Location.hpp"

#define	END_RES_LINE "\r\n"

class ResponseHeader
{
	public :

		ResponseHeader()
		{
		}

		virtual ~ResponseHeader()
		{

		}

		ResponseHeader(ResponseHeader const & rhs)
		{
			this->set_header(this->get_header());
			this->set_date(rhs.get_date());
			this->set_content_type(rhs.get_content_type());
			this->set_content_length(rhs.get_content_length());
			this->set_connection(rhs.get_connection());
			this->set_server_name(rhs.get_server_name());
			this->set_status(rhs.get_status());
			this->set_server(rhs.get_server());
			this->set_location(rhs.get_location());
		}

		ResponseHeader&		operator=(ResponseHeader const & rhs)
		{
			this->set_header(this->get_header());
			this->set_date(rhs.get_date());
			this->set_content_type(rhs.get_content_type());
			this->set_content_length(rhs.get_content_length());
			this->set_connection(rhs.get_connection());
			this->set_server_name(rhs.get_server_name());
			this->set_status(rhs.get_status());
			this->set_server(rhs.get_server());
			this->set_location(rhs.get_location());
			return (*this);
		}

		ResponseHeader(std::string const & server_name, bool connection,
				Server const & server, std::pair<bool, Location> const & location)
				: server_name(server_name), connection(connection), 
					server(server), location(location)
		{
			this->init_date();
		}

		// Setters


		void		set_header(std::string const & header)
		{
			this->header = header;
		}

		void		set_server_name(std::string const & server_name)
		{
			this->server_name = server_name;
		}

		void		set_date(std::string const & date)
		{
			this->date = date;
		}

		void		set_content_type(std::string const & content_type)
		{
			this->content_type = content_type;
		}

		void		set_content_length(size_t content_length)
		{
			this->content_length = content_length;
		}

		void		set_connection(bool connection)
		{
			this->connection = connection;
		}

		void		set_status(int status)
		{
			this->status = status;
		}

		void		set_server(Server const & server)
		{
			this->server = server;
		}

		void		set_location(std::pair<bool, Location> const & location)
		{
			this->location = location;
		}

		// Getters

		std::string	get_header(void) const
		{
			return (this->header);
		}

		std::string get_server_name(void) const
		{
			return (this->server_name);
		}

		std::string get_date(void) const
		{
			return (this->date);
		}

		std::string get_content_type(void) const
		{
			return (this->content_type);
		}

		size_t	 get_content_length(void) const
		{
			return (this->content_length);
		}

		bool			get_connection(void) const
		{
			return (this->connection);
		}

		int				get_status(void) const
		{
			return (this->status);
		}

		Server			get_server(void) const
		{
			return (this->server);
		}

		std::pair<bool, Location>		get_location(void) const
		{
			return (this->location);
		}

		void			init_date(void)
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
			this->set_date(date.str());
		}

		bool			is_redirection(unsigned int status)
		{
			return (std::find(this->redirection_status.begin(),
						this->redirection_status.end(),
						status) != this->redirection_status.end());
		}

		std::string		create_header(void)
		{
			std::stringstream		ss;

			ss << "Server: " << this->get_server_name() << END_RES_LINE;
			ss << "Date: " << this->get_date() << END_RES_LINE;
			ss << "Content-Type: " << this->get_content_type() << END_RES_LINE;
			ss << "Content-Length: " << this->get_content_length() << END_RES_LINE;
			if (this->is_redirection(this->get_status()) || this->get_status() == 300)
				ss << "Location: http://" << this->server.getHost() \
					<< ":" << this->server.getPort() << this->location.second.getReturnPath() << END_RES_LINE;
			ss << "Connection: " << this->get_connection_value() << END_RES_LINE;
			return (ss.str());
		}

		// statics

		static std::vector<std::string>		week_days;
		static std::vector<std::string>		months;
		static std::vector<int>				redirection_status;

	private :

		// whole header

		std::string				header;


		// header field

		std::string				server_name;
		std::string				date;
		std::string				content_type;
		size_t					content_length;
		bool					connection;

		int								status;
		Server							server;
		std::pair<bool, Location>		location;

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

		std::string	get_day(int day) const
		{
			std::stringstream		ss;

			if (day < 10)
				ss << 0;
			ss << day;
			return (ss.str());
		}

		std::string	get_week_day(int week_day) const
		{
			return (this->week_days[week_day]);
		}

		std::string	get_month(int month) const
		{
			return (this->months[month]);
		}

		int			get_year(int year) const
		{
			return (year + 1900);
		}

		std::string		get_connection_value(void)
		{
			if (this->get_connection())
				return ("keep-alive");
			else
				return ("close");
		}
};
