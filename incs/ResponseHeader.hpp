#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include <map>
#include <vector>

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
		}

		ResponseHeader&		operator=(ResponseHeader const & rhs)
		{
			this->set_header(this->get_header());
			this->set_date(rhs.get_date());
			this->set_content_type(rhs.get_content_type());
			this->set_content_length(rhs.get_content_length());
			this->set_connection(rhs.get_connection());
			this->set_server_name(rhs.get_server_name());
			return (*this);
		}

		ResponseHeader(std::string const & server_name, bool connection) : server_name(server_name), connection(connection)
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

		std::string		create_header(void)
		{
			std::stringstream		ss;

			ss << "Server: " << this->get_server_name() << END_RES_LINE;
			ss << "Date: " << this->get_date() << END_RES_LINE;
			ss << "Content-Type: " << this->get_content_type() << END_RES_LINE;
			ss << "Content-Length: " << this->get_content_length() << END_RES_LINE;
			return (ss.str());
		}

		// statics

		static std::vector<std::string>		week_days;
		static std::vector<std::string>		months;

	private :

		std::string				header;

		std::string				server_name;
		std::string				date;
		std::string				content_type;
		size_t					content_length;
		bool					connection;

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
};
