#pragma once 
#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <cstdlib>

struct Lower
{
	void		operator()(char & c)
	{
		c = std::tolower(c);
	}
};

class Request
{
	public :

		Request() {}

		Request(std::string const & request) : request(request), content_length(0)
		{
			std::string::iterator		it;
			
			try
			{
				it = this->parse_start_line();
				it = this->parse_header(it);
				it = this->parse_body(it);
				this->format = true;
			}
			catch (std::exception const & e)
			{
				this->format = false;
			}
		}

		virtual ~Request() {}

		Request(Request const & rhs)
		{
			this->set_request(rhs.get_request());
			this->set_source(rhs.get_source());
			this->set_type(rhs.get_type());
			this->set_version(rhs.get_version());
			this->set_connection(rhs.get_connection());
			this->set_content_type(rhs.get_content_type());
			this->set_content_length(rhs.get_content_length());
			this->set_body(rhs.get_body());
			this->set_format(rhs.get_format());
		}

		Request&		operator=(Request const & rhs)
		{
			this->set_request(rhs.get_request());
			this->set_source(rhs.get_source());
			this->set_type(rhs.get_type());
			this->set_version(rhs.get_version());
			this->set_connection(rhs.get_connection());
			this->set_content_type(rhs.get_content_type());
			this->set_content_length(rhs.get_content_length());
			this->set_body(rhs.get_body());
			this->set_format(rhs.get_format());
			return (*this);
		}

		// Getters

		std::string			get_type(void) const
		{
			return (this->type);
		}

		std::string			get_version(void) const
		{
			return (this->version);
		}

		const std::string			&get_request(void) const
		{
			return (this->request);
		}

		const std::string			&get_source(void) const
		{
			return (this->source);
		}

		bool						get_connection(void) const
		{
			return (this->connection);
		}

		bool						get_format(void) const
		{
			return (this->format);
		}

		std::string			get_content_type(void) const
		{
			return (this->content_type);
		}

		std::string			get_body(void) const
		{
			return (this->body);
		}

		size_t				get_content_length(void) const
		{
			return (this->content_length);
		}

		// Setters

		void	set_request(std::string const & request)
		{
			this->request = request;
		}

		void	set_type(std::string const & type)
		{
			this->type = type;
		}

		void	set_source(std::string const & source)
		{
			this->source = source;
		}

		void	set_version(std::string const & version)
		{
			this->version = version;
		}

		void	set_connection(bool connection)
		{
			this->connection = connection;
		}

		void	set_format(bool format)
		{
			this->format = format;
		}

		void	set_content_type(std::string const & content_type)
		{
			this->content_type = content_type;
		}

		void	set_body(std::string const & body)
		{
			this->body = body;
		}

		void	set_content_length(size_t content_length)
		{
			this->content_length = content_length;
		}

		class FormatException : public std::exception
		{
			public :
				FormatException() {};
				virtual const char* what() const throw()
				{
					return ("Error: missing space");
				};
		};

	protected :

		// general
	
		std::string				request;

		// first request line

		std::string				source;
		std::string				type;
		std::string				version;

		// request header

		bool					connection;
		std::string				content_type;
		size_t					content_length;

		std::string				body;

		// error

		bool					format;

		std::string		get_elem_at(int n)
		{
			std::string::const_iterator it = this->get_request().begin();
			std::string::const_iterator it2;
			
			for (int i = 0; i < n; i++)
			{
				while (*it == ' ')
				{
					it++;
				}
				it2 = it;
				while (*it != ' ' && it != this->get_request().end())
				{
					it++;
				}
			}
			return (get_request().substr(it2 - get_request().begin(), it - it2));
		}

		std::string::iterator		skip_space(std::string::iterator it)
		{
			while (it != request.end() && (*it == ' ' || *it == '\t'))
			{
				++it;
			}
			return (it);
		}

		std::string		read_string_in_request(std::string::iterator it, unsigned int i)
		{
			std::string		str;
			bool			format = false;

			while (it != this->request.end() && *it != ' ' && *it != '\t' && *it != '\r' && *it != '\n')
			{
				str.append(1, *it);
				format = true;
				++it;
			}
			if (!format && i != 2)
				throw (FormatException());
			return (str);
		}

		std::string::iterator		skip_end_line(std::string::iterator it)
		{
			std::pair<std::string::iterator, bool>		ret;

			ret.first = it;
			it = this->skip_space(it);
			if (*it == '\r')
			{
				++it;
				if (*it == '\n')
				{
					++it;
					ret.second = true;
					return (it);
				}
			}
			throw (FormatException());
		}

		std::string::iterator		parse_start_line(void)
		{
			std::string::iterator it = this->request.begin();

			for (unsigned int i = 0; i < 3; i++)
			{
				it = this->skip_space(it);
				switch (i)
				{
					case 0 :
						it += (this->type = this->read_string_in_request(it, i)).length();
						break;
					case 1:
						it += (this->source = this->read_string_in_request(it, i)).length();
						break;
					case 2:
						it += (this->version = this->read_string_in_request(it, i)).length();
				}
			}
			it = skip_end_line(it);
			return (it);
		}

		bool						end_of_request_line(std::string::iterator it)
		{
			if (*it == '\r')
			{
				++it;
				if (*it == '\n')
				{
					++it;
					return (true);
				}
			}
			return (false);
		}

		bool	is_number(std::string const & str)
		{
			return (str.find_first_not_of("0123456789") == std::string::npos);
		}

		void	assign_valid_fields(std::map<std::string, std::string> & fields)
		{
			if (!fields["connection"].compare("keep-alive"))
				this->set_connection(1);
			else
				this->set_connection(0);
			this->set_content_type(fields["content-type"]);
			if (this->is_number(fields["content-length"]))
				this->set_content_length(std::atol(fields["content-length"].c_str()));
		}

		void	print_header(void) const
		{
			std::cout << "header: " << std::endl;
			std::cout << "Connection: " << this->get_connection() << std::endl;
			std::cout << "Content-Type: " << this->get_content_type() << std::endl;
			std::cout << "Content-length: " << this->get_content_length() << std::endl;
		}

		std::string::iterator		parse_header(std::string::iterator it)
		{
			std::map<std::string, std::string>		fields;
			std::pair<std::string, std::string>		header_line;

			it = skip_space(it);
			while (!end_of_request_line(it))
			{

				while (*it != ':' && *it != '\r' && *it != '\n')
				{
					header_line.first.append(1, *it);
					++it;
				}
				if (*it == ':')
					++it;
				it = skip_space(it);
				while (!end_of_request_line(it))
				{
					header_line.second.append(1, *it);
					++it;
				}
				std::for_each(header_line.first.begin(), header_line.first.end(), Lower());
				std::for_each(header_line.second.begin(), header_line.second.end(), Lower());
				fields.insert(header_line);
				header_line.first.clear();
				header_line.second.clear();
				it = skip_end_line(it);
			}
			this->assign_valid_fields(fields);
			it = skip_end_line(it);
			
			this->print_header();
			return (it);
		}

		std::string::iterator		parse_body(std::string::iterator const & it)
		{
			std::string			body;

			body.append(it, this->request.end());
			this->set_body(body);
			return (it);
		}
};
